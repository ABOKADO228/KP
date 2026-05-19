#include <database/Database.hpp>
#include <database/Bootstrap.hpp>

#include <persistence/User.hpp>
#include <persistence/user-odb.hxx>

#include <memory>
#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>

#include <string>
#include <utility>

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

std::vector<fasc::server::persistence::User> Database::selectUsers() {
  return materialize<fasc::server::persistence::User>(
      underlying->query<fasc::server::persistence::User>(false));
}

std::optional<fasc::server::persistence::User> Database::findUserByLogin(
    const std::string& login) {
  using User = fasc::server::persistence::User;
  using UserQuery = odb::query<User>;

  std::vector<User> users = materialize<User>(underlying->query<User>(UserQuery::login == login));
  if (users.empty()) {
    return std::nullopt;
  }

  return std::move(users.front());
}

void Database::persistUser(fasc::server::persistence::User& user) {
  underlying->persist(user);
}

void Database::updateUser(fasc::server::persistence::User& user) {
  underlying->update(user);
}

} // namespace fasc::server::database
