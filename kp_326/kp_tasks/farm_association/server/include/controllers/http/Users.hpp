#pragma once

#include <controllers/app/Users.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Auth.hpp>
#include <views/Error.hpp>
#include <views/User.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using fasc::server::controllers::app::UserController;
using fasc::extended::fpp::Result;
using fasc::server::views::AuthView;
using fasc::server::views::ErrorView;
using fasc::server::views::UserView;

///Результат подготовки view для создания пользователя.
using UserViewResult = Result<UserView, ErrorView>;

///Результат подготовки view для регистрации или авторизации.
using AuthViewResult = Result<AuthView, ErrorView>;

///HTTP controller для пользовательских маршрутов.
///@note Преобразует команды и результаты application layer в публичные view без формирования HTTP-ответа.
class UserHttpController {
public:
  ///Создает HTTP controller поверх application controller.
  ///@param users application controller пользовательских сценариев.
  explicit UserHttpController(UserController& users);

  ///Создает пользователя по JSON-телу запроса.
  ///@param body JSON-тело запроса.
  ///@returns результат с публичным view пользователя или error view.
  UserViewResult createUser(std::string_view body);

  ///Регистрирует пользователя по JSON-телу запроса.
  ///@param body JSON-тело запроса.
  ///@returns результат с публичным auth view или error view.
  AuthViewResult registerUser(std::string_view body);

  ///Авторизует пользователя по JSON-телу запроса.
  ///@param body JSON-тело запроса.
  ///@returns результат с публичным auth view или error view.
  AuthViewResult loginUser(std::string_view body);

private:
  ///Преобразует application error в публичный error view.
  ///@param error ошибка application layer.
  ///@returns публичное представление ошибки.
  static ErrorView errorView(const fasc::server::controllers::app::UserError& error);

  ///Application controller пользовательских сценариев.
  UserController& users_;
};

} // namespace fasc::server::controllers::http
