#include <database/Database.hpp>

#include <memory>
#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>

#include <cstdlib>
#include <string>

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

} // namespace fasc::server::database
