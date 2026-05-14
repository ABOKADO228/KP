#include <database/Database.hpp>

#include <libpq-fe.h>

#include <odb/connection.hxx>
#include <memory>
#include <odb/database.hxx>
#include <odb/pgsql/connection.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

std::string envOr(const char* name, const char* fallback) {
  if (const char* value = std::getenv(name)) {
    return value;
  }

  return fallback;
}

unsigned int envPortOr(const char* name, unsigned int fallback) {
  if (const char* value = std::getenv(name)) {
    return static_cast<unsigned int>(std::stoul(value));
  }

  return fallback;
}

std::vector<const char*> parameterPointers(
    const std::vector<fasc::server::database::SqlParameter>& values) {
  std::vector<const char*> pointers;
  pointers.reserve(values.size());
  for (const auto& value : values) {
    pointers.push_back(value.isNull ? nullptr : value.text.c_str());
  }
  return pointers;
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
  return Database{std::make_unique<odb::pgsql::database>(
      envOr("FARM_DB_USER", "postgres"), envOr("FARM_DB_PASSWORD", "password"),
      envOr("FARM_DB_NAME", "farm_association"), envOr("FARM_DB_HOST", "localhost"),
      envPortOr("FARM_DB_PORT", 5432))};
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
