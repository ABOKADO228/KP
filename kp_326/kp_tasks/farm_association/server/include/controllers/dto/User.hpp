#pragma once

#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

inline constexpr const char* kAdministratorUserRole = "agriculture_admin";
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

/// Command for changing an existing user's role.
struct UpdateUserRoleCommand {
  /// Login of the user whose role should be changed.
  std::string login;

  /// New server-side access role code.
  std::string role;
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

/// Application-layer list of users.
struct UserListDto {
  /// Users known to the application.
  std::vector<UserDto> users;
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
