#pragma once

#include <server/core/AppRouter.hpp>

///HTTP-сервер приложения.
///@note Сейчас сервер принимает маршруты и запускает синхронный HTTP-loop на указанном порту.
class Server {
public:
  ///Регистрирует GET-маршрут.
  ///@param path    путь маршрута, например @c /health.
  ///@param handler обработчик маршрута.
  void get(std::string path, AppRouter::RouteHandler handler);

  ///Регистрирует POST-маршрут.
  ///@param path    путь маршрута, например @c /users.
  ///@param handler обработчик маршрута.
  void post(std::string path, AppRouter::RouteHandler handler);

  ///Запускает сервер на указанном TCP-порту.
  ///@param port порт, который должен слушать сервер.
  ///@returns код завершения server loop.
  ///@throws Исключения Boost.Asio/Boost.Beast при ошибках открытия сокета или сетевого ввода-вывода.
  int run(unsigned short port);

private:
  AppRouter router_;
};
