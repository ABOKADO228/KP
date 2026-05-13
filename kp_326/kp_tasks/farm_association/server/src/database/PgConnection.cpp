#include <database/PgConnection.hpp>

#include <cstdlib>
#include <stdexcept>
#include <string>

namespace fasc::server::database {
namespace {

std::string env_or(const char* name, const char* fallback) {
  if (const char* value = std::getenv(name)) {
    return value;
  }

  return fallback;
}

std::string connection_info_from_env() {
  return "host=" + env_or("FARM_DB_HOST", "localhost") +
         " port=" + env_or("FARM_DB_PORT", "5432") +
         " dbname=" + env_or("FARM_DB_NAME", "farm_association") +
         " user=" + env_or("FARM_DB_USER", "postgres") +
         " password=" + env_or("FARM_DB_PASSWORD", "password");
}

void ensure_ok(PGconn* connection, PGresult* result, const std::string& sql) {
  const ExecStatusType status = PQresultStatus(result);
  if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK) {
    throw std::runtime_error{"Database query failed: " + std::string{PQerrorMessage(connection)} +
                             " SQL: " + sql};
  }
}

} // namespace

void PgResultDeleter::operator()(PGresult* result) const {
  PQclear(result);
}

void PgConnection::ConnectionDeleter::operator()(PGconn* connection) const {
  PQfinish(connection);
}

PgConnection::PgConnection() : connection_(PQconnectdb(connection_info_from_env().c_str())) {
  if (connection_ == nullptr || PQstatus(connection_.get()) != CONNECTION_OK) {
    throw std::runtime_error{"Failed to connect PostgreSQL through libpq: " +
                             std::string{connection_ ? PQerrorMessage(connection_.get())
                                                     : "no connection object"}};
  }
}

PGconn* PgConnection::raw() {
  return connection_.get();
}

PgResultPtr PgConnection::execParams(const std::string& sql,
                                     const char* const* values,
                                     int value_count) {
  PgResultPtr result{PQexecParams(connection_.get(), sql.c_str(), value_count, nullptr, values,
                                  nullptr, nullptr, 0)};
  ensure_ok(connection_.get(), result.get(), sql);
  return result;
}

} // namespace fasc::server::database
