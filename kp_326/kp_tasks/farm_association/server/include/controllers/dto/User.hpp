#pragma once

#include <string>

///Команда application layer для создания пользователя.
///@note Используется как DTO входных данных после JSON unmarshalling.
struct CreateUserCommand {
  ///Имя создаваемого пользователя.
  std::string name;
};

///DTO пользователя, возвращаемый application layer.
///@note Не содержит HTTP-статус и не является persistence-объектом.
struct UserDto {
  ///Идентификатор пользователя в базе данных.
  unsigned long id{};

  ///Публичное имя пользователя.
  std::string name;
};
