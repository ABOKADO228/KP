#pragma once

#include <string>

namespace fasc::server::controllers::dto {

inline constexpr const char* kDefaultUserRole = "farm_worker";

/// Command for creating a user account from application code.
struct CreateUserCommand {
  /// Login. It is also the primary key in persistence.
  std::string login;

  /// Plain password from the incoming request.
  std::string password;

  /// Server-side access role code.
  std::string role{kDefaultUserRole};
};

/// Public self-registration command.
struct RegisterUserCommand {
  /// Login. It is also the primary key in persistence.
  std::string login;

  /// Plain password from the incoming request.
  std::string password;
};

/// Login command.
struct LoginUserCommand {
  /// Login. It is also the primary key in persistence.
  std::string login;

  /// Plain password from the incoming request.
  std::string password;
};

/// Application-layer user representation.
struct UserDto {
  /// Login. It is also the primary key in persistence.
  std::string login;

  /// Server-side access role code.
  std::string role{kDefaultUserRole};
};

/// Successful registration or login result.
struct AuthResultDto {
  /// User for whom the token was issued.
  UserDto user;

  /// JWT access token.
  std::string token;

  /// Token type for HTTP clients.
  std::string token_type{"Bearer"};
};

} // namespace fasc::server::controllers::dto
