#include <server/core/RequestDispatcher.hpp>

#include <string>

RequestDispatcher::RequestDispatcher(AppRouter& router) : router_(router) {}

BeastResponse RequestDispatcher::dispatch(const BeastRequest& request) {
  HttpRequest app_request;
  app_request.method = std::string(request.method_string());
  app_request.target = std::string(request.target());
  app_request.body = request.body();

  for (const auto& field : request) {
    app_request.headers.emplace(std::string(field.name_string()), std::string(field.value()));
  }

  const HttpResponse app_response = router_.route(app_request);

  BeastResponse response{static_cast<http::status>(app_response.status), request.version()};

  response.set(http::field::server, "farm-association-server");
  response.set(http::field::content_type, app_response.content_type);
  response.keep_alive(request.keep_alive());
  response.body() = app_response.body;
  response.prepare_payload();

  return response;
}
