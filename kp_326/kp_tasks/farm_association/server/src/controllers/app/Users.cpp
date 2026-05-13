#include <controllers/app/Users.hpp>

#include <persistence/User.hpp>
#include <persistence/user-odb.hxx>

#include <odb/database.hxx>
#include <odb/result.hxx>

#include <stdexcept>
#include <string>
#include <utility>

namespace {

void validate_name(const std::string& name) {
  if (name.empty()) {
    throw std::invalid_argument{"User name is required"};
  }
}

void validate_password(const std::string& password) {
  if (password.size() < 8) {
    throw std::invalid_argument{"Password must contain at least 8 characters"};
  }
}

} // namespace

UserController::UserController(fasc::server::database::Database& db,
                               fasc::server::security::PasswordHasher& password_hasher,
                               fasc::server::security::JwtService& jwt_service)
    : db_(db), password_hasher_(password_hasher), jwt_service_(jwt_service) {}

UserDto UserController::create_user(CreateUserCommand command) {
  return create_user_with_password(std::move(command.name), std::move(command.password));
}

AuthResultDto UserController::register_user(RegisterUserCommand command) {
  const UserDto user =
      create_user_with_password(std::move(command.name), std::move(command.password));

  AuthResultDto result;
  result.user = user;
  result.token = jwt_service_.issue(user);
  return result;
}

AuthResultDto UserController::login_user(LoginUserCommand command) {
  validate_name(command.name);
  validate_password(command.password);

  return db_.invokeTransactionally([&] {
    using query = odb::query<User>;

    odb::result<User> users = db_.raw().query<User>(query::name == command.name);
    auto iterator = users.begin();
    if (iterator == users.end()) {
      throw std::invalid_argument{"Invalid user name or password"};
    }

    const User user = *iterator;
    if (!password_hasher_.verify(command.password, user.password_hash())) {
      throw std::invalid_argument{"Invalid user name or password"};
    }

    AuthResultDto result;
    result.user = UserDto{user.id(), user.name()};
    result.token = jwt_service_.issue(result.user);
    return result;
  });
}

UserDto UserController::create_user_with_password(std::string name, std::string password) {
  validate_name(name);
  validate_password(password);

  return db_.invokeTransactionally([&] {
    using query = odb::query<User>;

    odb::result<User> existing = db_.raw().query<User>(query::name == name);
    if (existing.begin() != existing.end()) {
      throw std::invalid_argument{"User name is already registered"};
    }

    User user{std::move(name), password_hasher_.hash(password)};
    db_.persist(user);

    return UserDto{user.id(), user.name()};
  });
}
