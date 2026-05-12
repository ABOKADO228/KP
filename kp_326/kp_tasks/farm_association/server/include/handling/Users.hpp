#pragma once

#include <controllers/http/Users.hpp>
#include <server/core/HttpTypes.hpp>

///Handler пользовательских HTTP-маршрутов.
///@note Тонкая прослойка между router-ом и @c UserHttpController.
class UserHandler {
public:
  ///Создает handler поверх HTTP controller-а пользователей.
  ///@param users HTTP controller пользовательских маршрутов.
  explicit UserHandler(UserHttpController& users);

  ///Передает запрос создания пользователя в @c UserHttpController.
  ///@param request HTTP-запрос создания пользователя.
  ///@returns HTTP-ответ от пользовательского HTTP controller-а.
  ///@throws Исключения @c UserHttpController, если обработка запроса завершилась ошибкой.
  HttpResponse create_user(const HttpRequest& request);

private:
  UserHttpController& users_;
};
