#pragma once

#include <controllers/http/Users.hpp>
#include <server/core/HttpTypes.hpp>

namespace fasc::server::handling {

using fasc::server::controllers::http::UserHttpController;
using fasc::server::core::HttpRequest;
using fasc::server::core::HttpResponse;

///Handler пользовательских HTTP-маршрутов.
///@note Формирует @c HttpResponse из view, подготовленных @c UserHttpController.
class UserHandler {
public:
  ///Создает handler поверх HTTP controller-а пользователей.
  ///@param users HTTP controller пользовательских маршрутов.
  explicit UserHandler(UserHttpController& users);

  ///Формирует HTTP-ответ на запрос создания пользователя.
  ///@param request HTTP-запрос создания пользователя.
  ///@returns HTTP-ответ с JSON-телом.
  HttpResponse createUser(const HttpRequest& request);

  ///Формирует HTTP-ответ на запрос регистрации пользователя.
  ///@param request HTTP-запрос регистрации.
  ///@returns HTTP-ответ с JSON-телом.
  HttpResponse registerUser(const HttpRequest& request);

  ///Формирует HTTP-ответ на запрос авторизации пользователя.
  ///@param request HTTP-запрос авторизации.
  ///@returns HTTP-ответ с JSON-телом.
  HttpResponse loginUser(const HttpRequest& request);

private:
  ///HTTP controller пользовательских маршрутов.
  UserHttpController& users_;
};

} // namespace fasc::server::handling
