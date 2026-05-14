#pragma once

#include <string>

namespace fasc::server::controllers::dto {

///Команда создания пользователя.
///@note Используется внутри application layer и не является публичным JSON-контрактом.
struct CreateUserCommand {
  ///Имя пользователя.
  std::string name;

  ///Пароль пользователя в открытом виде из входного запроса.
  std::string password;
};

///Команда регистрации пользователя.
///@note После обработки пароль не сохраняется в открытом виде.
struct RegisterUserCommand {
  ///Имя пользователя.
  std::string name;

  ///Пароль пользователя в открытом виде из входного запроса.
  std::string password;
};

///Команда авторизации пользователя.
struct LoginUserCommand {
  ///Имя пользователя.
  std::string name;

  ///Пароль пользователя в открытом виде из входного запроса.
  std::string password;
};

///Внутреннее представление пользователя для application layer.
///@note Для HTTP-ответов преобразуется в @c UserView перед сериализацией.
struct UserDto {
  ///Идентификатор пользователя.
  unsigned long id{};

  ///Публичное имя пользователя.
  std::string name;
};

///Результат успешной регистрации или авторизации.
///@note Содержит application DTO и токен, но наружу отдается через view/json boundary.
struct AuthResultDto {
  ///Пользователь, для которого выпущен токен.
  UserDto user;

  ///JWT access token.
  std::string token;

  ///Тип токена для HTTP-клиентов.
  std::string token_type{"Bearer"};
};

} // namespace fasc::server::controllers::dto
