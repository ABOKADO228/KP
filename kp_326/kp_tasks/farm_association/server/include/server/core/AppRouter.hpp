#pragma once

#include <server/core/HttpTypes.hpp>

#include <functional>
#include <string>
#include <unordered_map>

namespace fasc::server::core {

///Маршрутизатор application layer для HTTP-запросов.
///@note Регистрирует обработчики по паре HTTP-метод и путь.
class AppRouter {
public:
  ///Функция-обработчик маршрута.
  ///@param request HTTP-запрос, пришедший на зарегистрированный маршрут.
  ///@returns HTTP-ответ, который будет отправлен клиенту.
  using RouteHandler = std::function<HttpResponse(const HttpRequest&)>;

  ///Регистрирует обработчик GET-маршрута.
  ///@param path    путь маршрута, например @c /health.
  ///@param handler обработчик, который будет вызван для этого пути.
  void get(std::string path, RouteHandler handler);

  ///Регистрирует обработчик POST-маршрута.
  ///@param path    путь маршрута, например @c /users.
  ///@param handler обработчик, который будет вызван для этого пути.
  void post(std::string path, RouteHandler handler);

  ///Регистрирует обработчик PUT-маршрута.
  ///@param path    путь маршрута.
  ///@param handler обработчик, который будет вызван для этого пути.
  void put(std::string path, RouteHandler handler);

  ///Регистрирует обработчик DELETE-маршрута.
  ///@param path    путь маршрута.
  ///@param handler обработчик, который будет вызван для этого пути.
  void del(std::string path, RouteHandler handler);

  ///Находит и вызывает обработчик для HTTP-запроса.
  ///@param request запрос, который нужно маршрутизировать.
  ///@returns ответ зарегистрированного обработчика или ответ об ошибке маршрутизации.
  HttpResponse route(const HttpRequest& request) const;

private:
  std::unordered_map<std::string, RouteHandler> routes_;

  ///Создает внутренний ключ маршрута из метода и пути.
  ///@param method HTTP-метод.
  ///@param path   путь запроса.
  ///@returns строковый ключ для хранения обработчика.
  static std::string key(std::string method, std::string path);
};

} // namespace fasc::server::core
