#pragma once

#include <controllers/app/Users.hpp>
#include <server/core/HttpTypes.hpp>
#include <views/User.hpp>

///HTTP controller для пользовательских маршрутов.
///@note Отвечает за JSON marshalling/unmarshalling и HTTP-статусы, бизнес-логику делегирует @c UserController.
class UserHttpController {
public:
  ///Создает HTTP controller поверх application controller.
  ///@param users application controller пользовательских сценариев.
  explicit UserHttpController(UserController& users);

  ///Обрабатывает HTTP-запрос на создание пользователя.
  ///@param request HTTP-запрос с JSON-телом.
  ///@returns HTTP-ответ с JSON-представлением созданного пользователя.
  ///@throws Исключения JSON parser-а или @c UserController, если тело запроса некорректно или пользователь не создан.
  HttpResponse create_user(const HttpRequest& request);

private:
  UserController& users_;
};
