#include <database/Database.hpp>
#include <database/Bootstrap.hpp>
#include <database/OdbTableRegistry.hpp>

#include <libpq-fe.h>

#include <odb/connection.hxx>
#include <memory>
#include <odb/database.hxx>
#include <odb/pgsql/connection.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>

#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::vector<const char*> parameterPointers(
    const std::vector<fasc::server::database::SqlParameter>& values) {
  std::vector<const char*> pointers;
  pointers.reserve(values.size());
  for (const auto& value : values) {
    pointers.push_back(value.isNull ? nullptr : value.text.c_str());
  }
  return pointers;
}

void requireIdentifierSegment(std::string_view identifier) {
  if (identifier.empty() ||
      !(std::isalpha(static_cast<unsigned char>(identifier.front())) ||
        identifier.front() == '_')) {
    throw std::invalid_argument{"Invalid SQL identifier: " + std::string{identifier}};
  }

  for (const char ch : identifier) {
    if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
      throw std::invalid_argument{"Invalid SQL identifier: " + std::string{identifier}};
    }
  }
}

std::string requireIdentifierPath(const std::string& identifier) {
  std::size_t start = 0;
  while (start <= identifier.size()) {
    const std::size_t dot = identifier.find('.', start);
    const std::string_view segment{
        identifier.data() + start,
        (dot == std::string::npos ? identifier.size() : dot) - start};
    requireIdentifierSegment(segment);

    if (dot == std::string::npos) {
      break;
    }
    start = dot + 1;
  }

  return identifier;
}

std::string columnsSql(const std::vector<std::string>& columns) {
  if (columns.empty()) {
    throw std::invalid_argument{"At least one column is required"};
  }

  std::string sql;
  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i != 0) {
      sql += ", ";
    }
    sql += requireIdentifierPath(columns[i]);
  }
  return sql;
}

std::string placeholdersSql(std::size_t count) {
  std::string sql;
  for (std::size_t i = 0; i < count; ++i) {
    if (i != 0) {
      sql += ", ";
    }
    sql += "$" + std::to_string(i + 1);
  }
  return sql;
}

std::string assignmentsSql(const std::vector<std::string>& columns) {
  if (columns.empty()) {
    throw std::invalid_argument{"At least one column is required"};
  }

  std::string sql;
  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i != 0) {
      sql += ", ";
    }
    sql += requireIdentifierPath(columns[i]) + " = $" + std::to_string(i + 1);
  }
  return sql;
}

std::string whereSql(const std::vector<std::string>& keyColumns, std::size_t offset = 0) {
  if (keyColumns.empty()) {
    throw std::invalid_argument{"At least one key column is required"};
  }

  std::string sql;
  for (std::size_t i = 0; i < keyColumns.size(); ++i) {
    if (i != 0) {
      sql += " AND ";
    }
    sql += requireIdentifierPath(keyColumns[i]) + " = $" + std::to_string(i + 1 + offset);
  }
  return sql;
}

void requireSameSize(std::string_view name,
                     const std::vector<std::string>& columns,
                     const std::vector<fasc::server::database::SqlParameter>& values) {
  if (columns.size() != values.size()) {
    throw std::invalid_argument{std::string{name} + " columns and values size mismatch"};
  }
}

} // namespace
namespace fasc::server::database {
Transaction::Transaction(odb::database& database)
    : transaction_(std::make_unique<odb::transaction>(database.begin())) {}

Transaction::~Transaction() noexcept = default;

void Transaction::commit() {
  if (transaction_ != nullptr) {
    transaction_->commit();
    transaction_.reset();
  }
}

void Transaction::rollback() {
  if (transaction_ != nullptr) {
    transaction_->rollback();
    transaction_.reset();
  }
}

Database::Database(std::unique_ptr<odb::database> database) : underlying(std::move(database)) {}

Database Database::createFromEnv() {
  return create(connectionSettingsFromEnv(compiledRuntimeDefaults().databaseName));
}

Database Database::create(ConnectionSettings settings) {
  return Database{std::make_unique<odb::pgsql::database>(
      std::move(settings.user), std::move(settings.password), std::move(settings.databaseName),
      std::move(settings.host), settings.port, "client_encoding=UTF8")};
}

Transaction Database::makeTransaction() {
  if (hasActiveTransaction()) {
    if (&odb::transaction::current().database() != underlying.get()) {
      throw odb::already_in_transaction();
    }

    return Transaction{};
  }

  return Transaction{*underlying};
}

bool Database::hasActiveTransaction() const {
  return odb::transaction::has_current();
}

odb::database& Database::raw() {
  return *underlying;
}

const odb::database& Database::raw() const {
  return *underlying;
}

std::vector<SqlRow> Database::selectRows(const std::string& table,
                                         const std::vector<std::string>& columns) {
  return selectOdbRows(*this, table, columns);
}

std::optional<SqlRow> Database::selectOneRow(const std::string& table,
                                             const std::vector<std::string>& columns,
                                             const std::vector<std::string>& keyColumns,
                                             const std::vector<SqlParameter>& keyValues) {
  return selectOneOdbRow(*this, table, columns, keyColumns, keyValues);
}

unsigned long long Database::insertRow(const std::string& table,
                                       const std::vector<std::string>& columns,
                                       const std::vector<SqlParameter>& values) {
  return insertOdbRow(*this, table, columns, values);
}

unsigned long long Database::updateRows(const std::string& table,
                                        const std::vector<std::string>& columns,
                                        const std::vector<SqlParameter>& values,
                                        const std::vector<std::string>& keyColumns,
                                        const std::vector<SqlParameter>& keyValues) {
  return updateOdbRows(*this, table, columns, values, keyColumns, keyValues);
}

unsigned long long Database::deleteRows(const std::string& table,
                                        const std::vector<std::string>& keyColumns,
                                        const std::vector<SqlParameter>& keyValues) {
  return deleteOdbRows(*this, table, keyColumns, keyValues);
}

std::vector<SqlRow> Database::querySql(const std::string& sql,
                                       const std::vector<SqlParameter>& values) {
  odb::connection_ptr owned_connection;
  odb::connection* connection = nullptr;
  if (hasActiveTransaction()) {
    connection = &odb::transaction::current().connection();
  } else {
    owned_connection = underlying->connection();
    connection = owned_connection.get();
  }

  auto* pg_connection = dynamic_cast<odb::pgsql::connection*>(connection);
  if (pg_connection == nullptr) {
    throw std::runtime_error{"Database connection is not PostgreSQL"};
  }

  const std::vector<const char*> pointers = parameterPointers(values);
  PGresult* raw_result = PQexecParams(pg_connection->handle(), sql.c_str(),
                                      static_cast<int>(pointers.size()), nullptr,
                                      pointers.data(), nullptr, nullptr, 0);
  std::unique_ptr<PGresult, decltype(&PQclear)> result{raw_result, &PQclear};
  if (result == nullptr || PQresultStatus(result.get()) != PGRES_TUPLES_OK) {
    throw std::runtime_error{PQerrorMessage(pg_connection->handle())};
  }

  std::vector<SqlRow> rows;
  rows.reserve(static_cast<std::size_t>(PQntuples(result.get())));
  for (int row_index = 0; row_index < PQntuples(result.get()); ++row_index) {
    SqlRow row;
    for (int column_index = 0; column_index < PQnfields(result.get()); ++column_index) {
      const std::string name = PQfname(result.get(), column_index);
      if (PQgetisnull(result.get(), row_index, column_index) == 1) {
        row.emplace(name, std::nullopt);
      } else {
        row.emplace(name, std::string{PQgetvalue(result.get(), row_index, column_index)});
      }
    }
    rows.push_back(std::move(row));
  }
  return rows;
}

unsigned long long Database::executeSql(const std::string& sql,
                                        const std::vector<SqlParameter>& values) {
  odb::connection_ptr owned_connection;
  odb::connection* connection = nullptr;
  if (hasActiveTransaction()) {
    connection = &odb::transaction::current().connection();
  } else {
    owned_connection = underlying->connection();
    connection = owned_connection.get();
  }

  auto* pg_connection = dynamic_cast<odb::pgsql::connection*>(connection);
  if (pg_connection == nullptr) {
    throw std::runtime_error{"Database connection is not PostgreSQL"};
  }

  const std::vector<const char*> pointers = parameterPointers(values);
  PGresult* raw_result = PQexecParams(pg_connection->handle(), sql.c_str(),
                                      static_cast<int>(pointers.size()), nullptr,
                                      pointers.data(), nullptr, nullptr, 0);
  std::unique_ptr<PGresult, decltype(&PQclear)> result{raw_result, &PQclear};
  if (result == nullptr || PQresultStatus(result.get()) != PGRES_COMMAND_OK) {
    throw std::runtime_error{PQerrorMessage(pg_connection->handle())};
  }

  const char* affected = PQcmdTuples(result.get());
  if (affected == nullptr || *affected == '\0') {
    return 0;
  }
  return std::stoull(affected);
}

} // namespace fasc::server::database
