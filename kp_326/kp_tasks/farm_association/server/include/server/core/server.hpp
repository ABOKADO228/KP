#pragma once

#include <server/core/AppRouter.hpp>

#include <chrono>
#include <cstddef>
#include <string>

struct ServerSettings {
  std::string address{"0.0.0.0"};
  unsigned short port{8080};
  std::size_t thread_count{0};
  std::size_t request_body_limit{1024 * 1024};
  std::chrono::seconds request_timeout{30};
  int listen_backlog{0};
};

/// HTTP server for application routes.
class Server {
public:
  /// Registers a GET route.
  void get(std::string path, AppRouter::RouteHandler handler);

  /// Registers a POST route.
  void post(std::string path, AppRouter::RouteHandler handler);
  void put(std::string path, AppRouter::RouteHandler handler);
  void del(std::string path, AppRouter::RouteHandler handler);

  /// Runs the server using the hardware concurrency as the worker count.
  int run(unsigned short port);

  /// Runs the server with an explicit number of Asio worker threads.
  int run(unsigned short port, std::size_t thread_count);

  /// Runs the server with production-oriented networking settings.
  int run(const ServerSettings& settings);

private:
  AppRouter router_;
};
