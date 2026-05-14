#include <controllers/app/Users.hpp>

#include <persistence/User.hpp>
#include <persistence/user-odb.hxx>

#include <odb/database.hxx>
#include <odb/result.hxx>

#include <stdexcept>
#include <string>
#include <utility>

namespace fasc::server::controllers::app {

using fasc::server::persistence::User;

namespace {

UserError validateName(const std::string& name) {
  if (name.empty()) {
    return UserError{UserErrorCode::InvalidInput, "User name is required"};
  }

  return UserError{};
}

UserError validatePassword(const std::string& password) {
  if (password.size() < 8) {
    return UserError{UserErrorCode::InvalidInput,
                     "Password must contain at least 8 characters"};
  }

  return UserError{};
}

} // namespace

UserController::UserController(fasc::server::database::Database& db,
                               fasc::server::security::PasswordHasher& passwordHasher,
                               fasc::server::security::JwtService& jwt_service)
    : db_(db), passwordHasher_(passwordHasher), jwt_service_(jwt_service) {}

CreateUserResult UserController::createUser(CreateUserCommand command) {
  return createUserWithPassword(std::move(command.name), std::move(command.password));
}

AuthResult UserController::registerUser(RegisterUserCommand command) {
  const CreateUserResult user_result =
      createUserWithPassword(std::move(command.name), std::move(command.password));
  if (user_result.hasError()) {
    return AuthResult::failure(user_result.error());
  }

  AuthResultDto result;
  result.user = user_result.success();
  result.token = jwt_service_.issue(result.user);
  return AuthResult::success(std::move(result));
}

AuthResult UserController::loginUser(LoginUserCommand command) {
  if (const UserError error = validateName(command.name); !error.message.empty()) {
    return AuthResult::failure(error);
  }

  if (const UserError error = validatePassword(command.password); !error.message.empty()) {
    return AuthResult::failure(error);
  }

  try {
    AuthResultDto result = db_.invokeTransactionally([&] {
    using query = odb::query<User>;

    odb::result<User> users = db_.raw().query<User>(query::name == command.name);
    auto iterator = users.begin();
    if (iterator == users.end()) {
      return AuthResultDto{};
    }

    const User user = *iterator;
    if (!passwordHasher_.verify(command.password, user.passwordHash())) {
      return AuthResultDto{};
    }

    AuthResultDto result;
    result.user = UserDto{user.id(), user.name()};
    result.token = jwt_service_.issue(result.user);
    return result;
  });

    if (result.token.empty()) {
      return AuthResult::failure(
          UserError{UserErrorCode::Unauthorized, "Invalid user name or password"});
    }

    return AuthResult::success(std::move(result));
  } catch (const std::exception& exception) {
    return AuthResult::failure(UserError{UserErrorCode::PersistenceFailure, exception.what()});
  }
}

CreateUserResult UserController::createUserWithPassword(std::string name,
                                                           std::string password) {
  if (const UserError error = validateName(name); !error.message.empty()) {
    return CreateUserResult::failure(error);
  }

  if (const UserError error = validatePassword(password); !error.message.empty()) {
    return CreateUserResult::failure(error);
  }

  try {
    UserDto user_dto = db_.invokeTransactionally([&] {
    using query = odb::query<User>;

    odb::result<User> existing = db_.raw().query<User>(query::name == name);
    if (existing.begin() != existing.end()) {
      return UserDto{};
    }

    User user{std::move(name), passwordHasher_.hash(password)};
    db_.persist(user);

    return UserDto{user.id(), user.name()};
  });

    if (user_dto.name.empty()) {
      return CreateUserResult::failure(
          UserError{UserErrorCode::Conflict, "User name is already registered"});
    }

    return CreateUserResult::success(std::move(user_dto));
  } catch (const std::exception& exception) {
    return CreateUserResult::failure(
        UserError{UserErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
