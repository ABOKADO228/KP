#include <database/Bootstrap.hpp>

#include <libpq-fe.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifndef FARM_SERVER_BUILD_CONFIG
#define FARM_SERVER_BUILD_CONFIG ""
#endif

#ifndef FARM_SERVER_DEFAULT_DB_NAME
#define FARM_SERVER_DEFAULT_DB_NAME "fasc_db"
#endif

#ifndef FARM_SERVER_RESET_DATABASE_ON_START
#define FARM_SERVER_RESET_DATABASE_ON_START 0
#endif

#ifndef FARM_SERVER_DOMAIN_SQL_PATH
#define FARM_SERVER_DOMAIN_SQL_PATH ""
#endif

#ifndef FARM_SERVER_USER_SQL_PATH
#define FARM_SERVER_USER_SQL_PATH ""
#endif

namespace {

using PgConnection = std::unique_ptr<PGconn, decltype(&PQfinish)>;
using PgResult = std::unique_ptr<PGresult, decltype(&PQclear)>;

std::string envOr(const char* name, std::string fallback) {
  if (const char* value = std::getenv(name)) {
    return value;
  }
  return fallback;
}

bool envBoolOr(const char* name, bool fallback) {
  const char* raw = std::getenv(name);
  if (raw == nullptr) {
    return fallback;
  }

  std::string value{raw};
  std::ranges::transform(value, value.begin(), [](unsigned char ch) {
    return static_cast<char>(std::tolower(ch));
  });

  if (value == "1" || value == "true" || value == "yes" || value == "on") {
    return true;
  }
  if (value == "0" || value == "false" || value == "no" || value == "off") {
    return false;
  }

  throw std::invalid_argument{std::string{name} + " must be a boolean value"};
}

unsigned int envPortOr(const char* name, unsigned int fallback) {
  if (const char* value = std::getenv(name)) {
    return static_cast<unsigned int>(std::stoul(value));
  }
  return fallback;
}

std::string trim(std::string_view value) {
  while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front())) != 0) {
    value.remove_prefix(1);
  }
  while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())) != 0) {
    value.remove_suffix(1);
  }
  return std::string{value};
}

std::string stripSqlComments(std::string_view statement) {
  std::string result;
  std::size_t start = 0;
  while (start <= statement.size()) {
    const std::size_t newline = statement.find('\n', start);
    const std::string_view line{
        statement.data() + start,
        (newline == std::string_view::npos ? statement.size() : newline) - start};
    const std::string trimmed = trim(line);
    if (!trimmed.starts_with("--")) {
      result += std::string{line};
      result += '\n';
    }
    if (newline == std::string_view::npos) {
      break;
    }
    start = newline + 1;
  }
  return result;
}

bool isCopyFromStdinStatement(std::string_view statement) {
  std::string uncommented = stripSqlComments(statement);
  std::ranges::transform(uncommented, uncommented.begin(), [](unsigned char ch) {
    return static_cast<char>(std::tolower(ch));
  });
  return trim(uncommented).starts_with("copy ") &&
         uncommented.find(" from stdin") != std::string::npos;
}

bool isCompleteSqlStatement(std::string_view sql) {
  bool single_quote = false;
  bool double_quote = false;
  bool line_comment = false;
  bool block_comment = false;

  for (std::size_t index = 0; index < sql.size(); ++index) {
    const char ch = sql[index];
    const char next = index + 1 < sql.size() ? sql[index + 1] : '\0';

    if (line_comment) {
      if (ch == '\n') {
        line_comment = false;
      }
      continue;
    }

    if (block_comment) {
      if (ch == '*' && next == '/') {
        block_comment = false;
        ++index;
      }
      continue;
    }

    if (!single_quote && !double_quote && ch == '-' && next == '-') {
      line_comment = true;
      ++index;
      continue;
    }

    if (!single_quote && !double_quote && ch == '/' && next == '*') {
      block_comment = true;
      ++index;
      continue;
    }

    if (!double_quote && ch == '\'') {
      if (single_quote && next == '\'') {
        ++index;
      } else {
        single_quote = !single_quote;
      }
      continue;
    }

    if (!single_quote && ch == '"') {
      double_quote = !double_quote;
      continue;
    }

    if (!single_quote && !double_quote && ch == ';') {
      return true;
    }
  }

  return false;
}

void requireDatabaseName(std::string_view databaseName) {
  if (databaseName.empty() ||
      !(std::isalpha(static_cast<unsigned char>(databaseName.front())) != 0 ||
        databaseName.front() == '_')) {
    throw std::invalid_argument{"Invalid database name: " + std::string{databaseName}};
  }

  for (const char ch : databaseName) {
    if (!(std::isalnum(static_cast<unsigned char>(ch)) != 0 || ch == '_')) {
      throw std::invalid_argument{"Invalid database name: " + std::string{databaseName}};
    }
  }
}

std::string escapeIdentifier(PGconn* connection, std::string_view identifier) {
  char* escaped = PQescapeIdentifier(connection, identifier.data(), identifier.size());
  if (escaped == nullptr) {
    throw std::runtime_error{PQerrorMessage(connection)};
  }
  std::string result{escaped};
  PQfreemem(escaped);
  return result;
}

PgConnection connectToPostgres(const fasc::server::database::ConnectionSettings& settings,
                               const std::string& databaseName) {
  const std::string port = std::to_string(settings.port);
  const char* keywords[] = {"host", "port", "user", "password", "dbname", nullptr};
  const char* values[] = {settings.host.c_str(), port.c_str(), settings.user.c_str(),
                          settings.password.c_str(), databaseName.c_str(), nullptr};

  PgConnection connection{PQconnectdbParams(keywords, values, 0), &PQfinish};
  if (connection == nullptr || PQstatus(connection.get()) != CONNECTION_OK) {
    throw std::runtime_error{connection != nullptr ? PQerrorMessage(connection.get())
                                                   : "Cannot allocate PostgreSQL connection"};
  }
  return connection;
}

PgResult execParams(PGconn* connection,
                    const std::string& sql,
                    const std::vector<const char*>& values = {}) {
  PgResult result{PQexecParams(connection, sql.c_str(), static_cast<int>(values.size()), nullptr,
                               values.data(), nullptr, nullptr, 0),
                  &PQclear};
  if (result == nullptr) {
    throw std::runtime_error{PQerrorMessage(connection)};
  }
  return result;
}

void requireStatus(PGconn* connection,
                   const PGresult* result,
                   ExecStatusType expected,
                   const std::string& sql) {
  if (PQresultStatus(result) != expected) {
    const char* message = PQresultErrorMessage(result);
    throw std::runtime_error{std::string{"SQL bootstrap failed for statement: "} + sql +
                             "\n" + (message != nullptr && *message != '\0'
                                          ? message
                                          : PQerrorMessage(connection))};
  }
}

void executeCommand(PGconn* connection, const std::string& sql) {
  PgResult result = execParams(connection, sql);
  const ExecStatusType status = PQresultStatus(result.get());
  if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK && status != PGRES_EMPTY_QUERY) {
    requireStatus(connection, result.get(), PGRES_COMMAND_OK, sql);
  }
}

bool databaseExists(PGconn* connection, const std::string& databaseName) {
  const char* value = databaseName.c_str();
  PgResult result = execParams(connection,
                               "SELECT 1 FROM pg_database WHERE datname = $1 LIMIT 1",
                               {value});
  requireStatus(connection, result.get(), PGRES_TUPLES_OK, "database existence check");
  return PQntuples(result.get()) > 0;
}

void terminateDatabaseConnections(PGconn* connection, const std::string& databaseName) {
  const char* value = databaseName.c_str();
  PgResult result = execParams(
      connection,
      "SELECT pg_terminate_backend(pid) "
      "FROM pg_stat_activity "
      "WHERE datname = $1 AND pid <> pg_backend_pid()",
      {value});
  requireStatus(connection, result.get(), PGRES_TUPLES_OK, "terminate database connections");
}

void dropDatabase(PGconn* connection, const std::string& databaseName) {
  terminateDatabaseConnections(connection, databaseName);
  executeCommand(connection, "DROP DATABASE IF EXISTS " + escapeIdentifier(connection, databaseName));
}

void createDatabase(PGconn* connection, const std::string& databaseName) {
  executeCommand(connection, "CREATE DATABASE " + escapeIdentifier(connection, databaseName));
}

bool tableExists(PGconn* connection, const std::string& tableName) {
  const char* schema = "public";
  const char* table = tableName.c_str();
  PgResult result = execParams(
      connection,
      "SELECT 1 FROM information_schema.tables "
      "WHERE table_schema = $1 AND table_name = $2 LIMIT 1",
      {schema, table});
  requireStatus(connection, result.get(), PGRES_TUPLES_OK, "table existence check");
  return PQntuples(result.get()) > 0;
}

std::string readFile(const std::filesystem::path& path) {
  std::ifstream input{path, std::ios::binary};
  if (!input) {
    throw std::runtime_error{"Cannot open SQL bootstrap script: " + path.string()};
  }

  return {std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
}

void finishCopy(PGconn* connection, const std::string& statement) {
  if (PQputCopyEnd(connection, nullptr) != 1) {
    throw std::runtime_error{PQerrorMessage(connection)};
  }

  while (PGresult* raw = PQgetResult(connection)) {
    PgResult result{raw, &PQclear};
    requireStatus(connection, result.get(), PGRES_COMMAND_OK, statement);
  }
}

void executeSqlScript(PGconn* connection, const std::filesystem::path& path) {
  const std::string script = readFile(path);
  std::string statement;
  bool copy_mode = false;
  std::string copy_statement;

  std::size_t start = 0;
  while (start <= script.size()) {
    std::size_t newline = script.find('\n', start);
    std::string line = newline == std::string::npos
                           ? script.substr(start)
                           : script.substr(start, newline - start);
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }

    if (copy_mode) {
      if (line == "\\.") {
        finishCopy(connection, copy_statement);
        copy_mode = false;
        copy_statement.clear();
      } else {
        const std::string copy_line = line + '\n';
        if (PQputCopyData(connection, copy_line.data(),
                          static_cast<int>(copy_line.size())) != 1) {
          throw std::runtime_error{PQerrorMessage(connection)};
        }
      }
    } else {
      statement += line;
      statement += '\n';

      if (isCompleteSqlStatement(statement)) {
        const std::string sql = trim(statement);
        if (!sql.empty()) {
          if (isCopyFromStdinStatement(sql)) {
            PgResult result{PQexec(connection, sql.c_str()), &PQclear};
            if (result == nullptr) {
              throw std::runtime_error{PQerrorMessage(connection)};
            }
            requireStatus(connection, result.get(), PGRES_COPY_IN, sql);
            copy_mode = true;
            copy_statement = sql;
          } else {
            executeCommand(connection, sql);
          }
        }
        statement.clear();
      }
    }

    if (newline == std::string::npos) {
      break;
    }
    start = newline + 1;
  }

  if (copy_mode) {
    throw std::runtime_error{"Unfinished COPY block in SQL bootstrap script: " + path.string()};
  }

  const std::string trailing = trim(statement);
  if (!trailing.empty()) {
    executeCommand(connection, trailing);
  }
}

std::vector<fasc::server::database::DatabaseBootstrapScript> defaultScriptPaths() {
  using fasc::server::database::DatabaseBootstrapScriptKind;

  std::vector<fasc::server::database::DatabaseBootstrapScript> scripts;

  std::string domainScript = envOr("FARM_DB_SCHEMA_SQL", FARM_SERVER_DOMAIN_SQL_PATH);
  std::string userScript = envOr("FARM_DB_USER_SQL", FARM_SERVER_USER_SQL_PATH);

  if (!domainScript.empty()) {
    scripts.push_back(
        {DatabaseBootstrapScriptKind::DomainSchema, std::filesystem::path{domainScript}});
  }
  if (!userScript.empty()) {
    scripts.push_back({DatabaseBootstrapScriptKind::UserSchema, std::filesystem::path{userScript}});
  }

  return scripts;
}

} // namespace

namespace fasc::server::database {

DatabaseRuntimeDefaults defaultsForBuildConfig(std::string_view buildConfig) {
  if (buildConfig == "Debug") {
    return DatabaseRuntimeDefaults{"fasc_test", DatabaseBootstrapMode::ResetAndInitialize};
  }
  return DatabaseRuntimeDefaults{"fasc_db", DatabaseBootstrapMode::EnsureInitialized};
}

DatabaseRuntimeDefaults compiledRuntimeDefaults() {
  return DatabaseRuntimeDefaults{
      FARM_SERVER_DEFAULT_DB_NAME,
      FARM_SERVER_RESET_DATABASE_ON_START != 0 ? DatabaseBootstrapMode::ResetAndInitialize
                                               : DatabaseBootstrapMode::EnsureInitialized};
}

ConnectionSettings connectionSettingsFromEnv(std::string defaultDatabaseName) {
  return ConnectionSettings{envOr("FARM_DB_USER", "postgres"),
                            envOr("FARM_DB_PASSWORD", "password"),
                            envOr("FARM_DB_NAME", std::move(defaultDatabaseName)),
                            envOr("FARM_DB_HOST", "localhost"),
                            envPortOr("FARM_DB_PORT", 5432)};
}

DatabaseBootstrapOptions bootstrapOptionsFromEnv() {
  const DatabaseRuntimeDefaults defaults = compiledRuntimeDefaults();
  DatabaseBootstrapOptions options;
  options.enabled = envBoolOr("FARM_DB_BOOTSTRAP", true);
  options.connection = connectionSettingsFromEnv(defaults.databaseName);
  options.maintenanceDatabase = envOr("FARM_DB_MAINTENANCE_NAME", "postgres");
  options.mode = envBoolOr("FARM_DB_RESET_ON_START",
                           defaults.mode == DatabaseBootstrapMode::ResetAndInitialize)
                     ? DatabaseBootstrapMode::ResetAndInitialize
                     : DatabaseBootstrapMode::EnsureInitialized;
  options.initializationScripts = defaultScriptPaths();
  return options;
}

void prepareDatabase(const DatabaseBootstrapOptions& options) {
  if (!options.enabled) {
    return;
  }

  requireDatabaseName(options.connection.databaseName);
  requireDatabaseName(options.maintenanceDatabase);
  if (options.connection.databaseName == options.maintenanceDatabase) {
    throw std::invalid_argument{"Maintenance database must differ from target database"};
  }

  PgConnection maintenance = connectToPostgres(options.connection, options.maintenanceDatabase);
  const bool reset = options.mode == DatabaseBootstrapMode::ResetAndInitialize;

  if (reset) {
    dropDatabase(maintenance.get(), options.connection.databaseName);
    createDatabase(maintenance.get(), options.connection.databaseName);
  } else if (!databaseExists(maintenance.get(), options.connection.databaseName)) {
    createDatabase(maintenance.get(), options.connection.databaseName);
  }

  PgConnection target = connectToPostgres(options.connection, options.connection.databaseName);

  const bool hasDomainSchema = tableExists(target.get(), "farm");
  const bool hasUserSchema = tableExists(target.get(), "User");

  for (const auto& script : options.initializationScripts) {
    const bool userScript = script.kind == DatabaseBootstrapScriptKind::UserSchema;
    const bool shouldRun = reset || (userScript ? !hasUserSchema : !hasDomainSchema);
    if (shouldRun) {
      executeSqlScript(target.get(), script.path);
    }
  }
}

} // namespace fasc::server::database
