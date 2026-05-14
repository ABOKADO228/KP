#pragma once

#include <server/core/AppRouter.hpp>

#include <chrono>
#include <cstddef>
#include <string>

namespace fasc::server::core {

///Настройки запуска HTTP-сервера.
///@note Значения по умолчанию подходят для локального запуска разработки.
struct ServerSettings {
  ///Адрес, на котором сервер принимает входящие подключения.
  std::string address{"0.0.0.0"};

  ///TCP-порт сервера.
  unsigned short port{8080};

  ///Количество рабочих потоков Asio; @c 0 означает auto-detect.
  std::size_t thread_count{0};

  ///Максимальный размер HTTP-тела в байтах.
  std::size_t request_body_limit{1024 * 1024};

  ///Таймаут чтения и записи одного HTTP-запроса.
  std::chrono::seconds request_timeout{30};

  ///Размер listen backlog; @c 0 означает системное значение Boost.Asio.
  int listen_backlog{0};
};

///HTTP-сервер для application routes.
class Server {
public:
  ///Регистрирует GET-маршрут.
  ///@param path    путь маршрута.
  ///@param handler обработчик маршрута.
  void get(std::string path, AppRouter::RouteHandler handler);

  ///Регистрирует POST-маршрут.
  ///@param path    путь маршрута.
  ///@param handler обработчик маршрута.
  void post(std::string path, AppRouter::RouteHandler handler);

  ///Регистрирует PUT-маршрут.
  ///@param path    путь маршрута.
  ///@param handler обработчик маршрута.
  void put(std::string path, AppRouter::RouteHandler handler);

  ///Регистрирует DELETE-маршрут.
  ///@param path    путь маршрута.
  ///@param handler обработчик маршрута.
  void del(std::string path, AppRouter::RouteHandler handler);

  ///Запускает сервер на указанном порту с автоматическим выбором числа потоков.
  ///@param port TCP-порт сервера.
  ///@returns код завершения процесса.
  int run(unsigned short port);

  ///Запускает сервер на указанном порту с заданным числом рабочих потоков.
  ///@param port         TCP-порт сервера.
  ///@param thread_count число рабочих потоков Asio.
  ///@returns код завершения процесса.
  int run(unsigned short port, std::size_t thread_count);

  ///Запускает сервер с полным набором сетевых настроек.
  ///@param settings настройки запуска.
  ///@returns код завершения процесса.
  int run(const ServerSettings& settings);

private:
  AppRouter router_;
};

} // namespace fasc::server::core
