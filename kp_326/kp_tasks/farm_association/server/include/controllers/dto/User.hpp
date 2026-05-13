#pragma once

#include <string>

struct CreateUserCommand {
  std::string name;
  std::string password;
};

struct RegisterUserCommand {
  std::string name;
  std::string password;
};

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
