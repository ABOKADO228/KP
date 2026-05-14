#pragma once

#include <views/User.hpp>

#include <string>

namespace fasc::server::views {

///Публичное представление успешной регистрации или авторизации.
///@note Содержит токен и публичный view пользователя.
struct AuthView {
  ///JWT access token.
  std::string token;

  ///Тип токена для HTTP-клиентов.
  std::string token_type;

  ///Публичное представление пользователя.
  UserView user;
};

} // namespace fasc::server::views
