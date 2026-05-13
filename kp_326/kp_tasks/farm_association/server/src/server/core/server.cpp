#include <server/core/Server.hpp>

#include <server/core/RequestDispatcher.hpp>

#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace {

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

std::mutex log_mutex;

void log_info(const std::string& message) {
  std::lock_guard lock{log_mutex};
  std::clog << "[server] " << message << '\n';
}

std::string error_body(std::string message) {
  nlohmann::json body;
  body["error"] = std::move(message);
  return body.dump();
}

BeastResponse make_error_response(http::status status, std::string message, unsigned version,
                                  bool keep_alive) {
  BeastResponse response{status, version};
  response.set(http::field::server, "farm-association-server");
  response.set(http::field::content_type, "application/json");
  response.set("X-Content-Type-Options", "nosniff");
  response.keep_alive(keep_alive);
  response.body() = error_body(std::move(message));
  response.prepare_payload();
  return response;
}

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
  HttpSession(tcp::socket socket, const RequestDispatcher& dispatcher,
              const ServerSettings& settings)
      : stream_(std::move(socket)), dispatcher_(dispatcher), settings_(settings) {}

  void run() {
    read_next_request();
  }

private:
  using RequestParser = http::request_parser<http::string_body>;

  void read_next_request() {
    parser_ = std::make_unique<RequestParser>();
    parser_->body_limit(settings_.request_body_limit);

    stream_.expires_after(settings_.request_timeout);
    http::async_read(stream_, buffer_, *parser_,
                     beast::bind_front_handler(&HttpSession::on_read, shared_from_this()));
  }

  void on_read(beast::error_code error, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (error == http::error::end_of_stream) {
      shutdown();
      return;
    }

    if (error) {
      write_protocol_error(error);
      return;
    }

    response_ = std::make_shared<BeastResponse>(dispatcher_.dispatch(parser_->get()));
    parser_.reset();
    write_response(response_->keep_alive());
  }

  void write_protocol_error(const beast::error_code& error) {
    const auto status = error == http::error::body_limit ? http::status::payload_too_large
                                                         : http::status::bad_request;
    response_ = std::make_shared<BeastResponse>(
        make_error_response(status, error.message(), 11, false));
    parser_.reset();
    write_response(false);
  }

  void write_response(bool keep_alive) {
    stream_.expires_after(settings_.request_timeout);
    http::async_write(stream_, *response_,
                      beast::bind_front_handler(&HttpSession::on_write, shared_from_this(),
                                                keep_alive));
  }

  void on_write(bool keep_alive, beast::error_code error, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    response_.reset();

    if (error) {
      return;
    }

    if (!keep_alive) {
      shutdown();
      return;
    }

    read_next_request();
  }

  void shutdown() {
    beast::error_code ignored;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ignored);
  }

  beast::tcp_stream stream_;
  beast::flat_buffer buffer_;
  std::unique_ptr<RequestParser> parser_;
  std::shared_ptr<BeastResponse> response_;
  const RequestDispatcher& dispatcher_;
  const ServerSettings& settings_;
};

class Listener : public std::enable_shared_from_this<Listener> {
public:
  Listener(asio::io_context& io, tcp::endpoint endpoint, const RequestDispatcher& dispatcher,
           const ServerSettings& settings)
      : io_(io), acceptor_(io), dispatcher_(dispatcher), settings_(settings) {
    beast::error_code error;

    acceptor_.open(endpoint.protocol(), error);
    throw_if_error(error, "open");

    acceptor_.set_option(asio::socket_base::reuse_address(true), error);
    throw_if_error(error, "set_option");

    acceptor_.bind(endpoint, error);
    throw_if_error(error, "bind");

    const int backlog = settings_.listen_backlog > 0 ? settings_.listen_backlog
                                                     : asio::socket_base::max_listen_connections;
    acceptor_.listen(backlog, error);
    throw_if_error(error, "listen");
  }

  void run() {
    accept_next();
  }

private:
  static void throw_if_error(const beast::error_code& error, const char* operation) {
    if (error) {
      throw beast::system_error{error, operation};
    }
  }

  void accept_next() {
    acceptor_.async_accept(beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
  }

  void on_accept(beast::error_code error, tcp::socket socket) {
    if (!error) {
      std::make_shared<HttpSession>(std::move(socket), dispatcher_, settings_)->run();
    }

    if (error && error != asio::error::operation_aborted) {
      log_info(std::string{"accept failed: "} + error.message());
    }

    if (acceptor_.is_open()) {
      accept_next();
    }
  }

  asio::io_context& io_;
  tcp::acceptor acceptor_;
  const RequestDispatcher& dispatcher_;
  const ServerSettings& settings_;
};

std::size_t default_thread_count() {
  return std::max<std::size_t>(1, std::thread::hardware_concurrency());
}

std::size_t normalize_thread_count(std::size_t thread_count) {
  if (thread_count == 0) {
    return default_thread_count();
  }

  return std::max<std::size_t>(1, thread_count);
}

ServerSettings settings_for_port(unsigned short port, std::size_t thread_count) {
  ServerSettings settings;
  settings.port = port;
  settings.thread_count = thread_count;
  return settings;
}

tcp::endpoint make_endpoint(const ServerSettings& settings) {
  beast::error_code error;

  const auto ipv4_address = asio::ip::make_address_v4(settings.address, error);
  if (!error) {
    return tcp::endpoint{ipv4_address, settings.port};
  }

  const auto ipv6_address = asio::ip::make_address_v6(settings.address, error);
  if (!error) {
    return tcp::endpoint{ipv6_address, settings.port};
  }

  throw beast::system_error{error, "address"};
}

} // namespace

void Server::get(std::string path, AppRouter::RouteHandler handler) {
  router_.get(std::move(path), std::move(handler));
}

void Server::post(std::string path, AppRouter::RouteHandler handler) {
  router_.post(std::move(path), std::move(handler));
}

void Server::put(std::string path, AppRouter::RouteHandler handler) {
  router_.put(std::move(path), std::move(handler));
}

void Server::del(std::string path, AppRouter::RouteHandler handler) {
  router_.del(std::move(path), std::move(handler));
}

int Server::run(unsigned short port) {
  return run(settings_for_port(port, 0));
}

int Server::run(unsigned short port, std::size_t thread_count) {
  return run(settings_for_port(port, thread_count));
}

int Server::run(const ServerSettings& settings) {
  const std::size_t worker_count = normalize_thread_count(settings.thread_count);

  asio::io_context io{static_cast<int>(worker_count)};
  RequestDispatcher dispatcher{router_};
  const tcp::endpoint endpoint = make_endpoint(settings);

  std::make_shared<Listener>(io, endpoint, dispatcher, settings)->run();

  asio::signal_set signals{io, SIGINT, SIGTERM};
  signals.async_wait([&io](const beast::error_code& error, int signal_number) {
    if (!error) {
      log_info("shutdown signal received: " + std::to_string(signal_number));
      io.stop();
    }
  });

  log_info("listening on " + endpoint.address().to_string() + ":" +
           std::to_string(endpoint.port()) + ", workers=" + std::to_string(worker_count) +
           ", body_limit=" + std::to_string(settings.request_body_limit));

  std::vector<std::thread> workers;
  workers.reserve(worker_count);

  std::exception_ptr worker_exception;
  std::mutex exception_mutex;

  for (std::size_t i = 0; i < worker_count; ++i) {
    workers.emplace_back([&io, &worker_exception, &exception_mutex] {
      try {
        io.run();
      } catch (...) {
        {
          std::lock_guard lock{exception_mutex};
          if (worker_exception == nullptr) {
            worker_exception = std::current_exception();
          }
        }
        io.stop();
      }
    });
  }

  for (auto& worker : workers) {
    worker.join();
  }

  if (worker_exception != nullptr) {
    std::rethrow_exception(worker_exception);
  }

  log_info("stopped");
  return 0;
}
