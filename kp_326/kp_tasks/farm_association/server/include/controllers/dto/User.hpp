#pragma once

#include <string>

/// DTO команды создания пользователя
struct CreateUserCommand {
  std::string name;
  std::string password;
};

/// DTO команды регистрации пользователя
struct RegisterUserCommand {
  std::string name;
  std::string password;
};

/// DTO команды авторизации пользователя
struct LoginUserCommand {
  std::string name;
  std::string password;
};

struct UserDto {
  unsigned long id{};
  std::string name;
};

struct AuthResultDto {
  UserDto user;
  std::string token;
  std::string token_type{"Bearer"};
};
