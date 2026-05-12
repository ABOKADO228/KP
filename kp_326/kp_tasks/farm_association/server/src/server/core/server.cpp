#include <server/core/Server.hpp>

#include <server/core/RequestDispatcher.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <utility>

void Server::get(std::string path, AppRouter::RouteHandler handler) {
  router_.get(std::move(path), std::move(handler));
}

void Server::post(std::string path, AppRouter::RouteHandler handler) {
  router_.post(std::move(path), std::move(handler));
}

int Server::run(unsigned short port) {
  namespace asio = boost::asio;
  namespace http = boost::beast::http;
  using tcp = asio::ip::tcp;

  asio::io_context io;
  tcp::acceptor acceptor{io, {tcp::v4(), port}};
  RequestDispatcher dispatcher{router_};

  for (;;) {
    tcp::socket socket{io};
    acceptor.accept(socket);

    boost::beast::flat_buffer buffer;
    BeastRequest request;
    http::read(socket, buffer, request);

    BeastResponse response = dispatcher.dispatch(request);
    http::write(socket, response);
  }
}
