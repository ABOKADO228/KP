#include <controllers/app/Users.hpp>

#include <persistence/User.hpp>
#include <persistence/user-odb.hxx>

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

using fasc::server::controllers::dto::kDefaultUserRole;
using fasc::server::persistence::User;

namespace {

constexpr std::array<std::string_view, 7> kKnownUserRoles{
    "agriculture_admin",
    "association_director",
    "farm_owner",
    "agronomist",
    "procurement_manager",
    "sales_manager",
    "farm_worker",
};

bool isKnownUserRole(std::string_view role) {
  for (std::string_view knownRole : kKnownUserRoles) {
    if (role == knownRole) {
      return true;
    }
  }

  return false;
}

std::string normalizeRole(std::string role) {
  if (role.empty()) {
    return std::string{kDefaultUserRole};
  }

  return role;
}

UserError validateLogin(const std::string& login) {
  if (login.empty()) {
    return UserError{UserErrorCode::InvalidInput, "User login is required"};
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

UserError validateRole(const std::string& role) {
  if (!isKnownUserRole(role)) {
    return UserError{UserErrorCode::InvalidInput, "User role is invalid"};
  }

  return UserError{};
}

} // namespace

UserController::UserController(fasc::server::database::Database& db,
                               fasc::server::security::PasswordHasher& passwordHasher,
                               fasc::server::security::JwtService& jwt_service)
    : db_(db), passwordHasher_(passwordHasher), jwt_service_(jwt_service) {}

CreateUserResult UserController::createUser(CreateUserCommand command) {
  return createUserWithPassword(std::move(command.login),
                                std::move(command.password),
                                std::move(command.role));
}

AuthResult UserController::registerUser(RegisterUserCommand command) {
  const CreateUserResult userResult =
      createUserWithPassword(std::move(command.login),
                             std::move(command.password),
                             std::string{kDefaultUserRole});
  if (userResult.hasError()) {
    return AuthResult::failure(userResult.error());
  }

  AuthResultDto result;
  result.user = userResult.success();
  result.token = jwt_service_.issue(result.user);
  return AuthResult::success(std::move(result));
}

AuthResult UserController::loginUser(LoginUserCommand command) {
  if (const UserError error = validateLogin(command.login); !error.message.empty()) {
    return AuthResult::failure(error);
  }

  if (const UserError error = validatePassword(command.password); !error.message.empty()) {
    return AuthResult::failure(error);
  }

  try {
    AuthResultDto result = db_.invokeTransactionally([&] {
      using query = odb::query<User>;

      std::vector<User> users = db_.query<User>(query::login == command.login);
      if (users.empty()) {
        return AuthResultDto{};
      }

      const User& user = users.front();
      if (!passwordHasher_.verify(command.password, user.passwordHash())) {
        return AuthResultDto{};
      }

      AuthResultDto authResult;
      authResult.user = UserDto{user.login(), user.role()};
      authResult.token = jwt_service_.issue(authResult.user);
      return authResult;
    });

    if (result.token.empty()) {
      return AuthResult::failure(
          UserError{UserErrorCode::Unauthorized, "Invalid user login or password"});
    }

    return AuthResult::success(std::move(result));
  } catch (const std::exception& exception) {
    return AuthResult::failure(UserError{UserErrorCode::PersistenceFailure, exception.what()});
  }
}

CreateUserResult UserController::createUserWithPassword(std::string login,
                                                        std::string password,
                                                        std::string role) {
  role = normalizeRole(std::move(role));

  if (const UserError error = validateLogin(login); !error.message.empty()) {
    return CreateUserResult::failure(error);
  }

  if (const UserError error = validatePassword(password); !error.message.empty()) {
    return CreateUserResult::failure(error);
  }

  if (const UserError error = validateRole(role); !error.message.empty()) {
    return CreateUserResult::failure(error);
  }

  try {
    UserDto userDto = db_.invokeTransactionally([&] {
      using query = odb::query<User>;

      std::vector<User> existing = db_.query<User>(query::login == login);
      if (!existing.empty()) {
        return UserDto{};
      }

      User user{std::move(login), passwordHasher_.hash(password), std::move(role)};
      db_.persist(user);

      return UserDto{user.login(), user.role()};
    });

    if (userDto.login.empty()) {
      return CreateUserResult::failure(
          UserError{UserErrorCode::Conflict, "User login is already registered"});
    }

    return CreateUserResult::success(std::move(userDto));
  } catch (const std::exception& exception) {
    return CreateUserResult::failure(
        UserError{UserErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
