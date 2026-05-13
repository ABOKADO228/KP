#include <server/core/AppRouter.hpp>

#include <nlohmann/json.hpp>

#include <exception>
#include <string>
#include <utility>

namespace {

std::string error_body(std::string message) {
  nlohmann::json body;
  body["error"] = std::move(message);
  return body.dump();
}

} // namespace

void AppRouter::get(std::string path, RouteHandler handler) {
  routes_.emplace(key("GET", std::move(path)), std::move(handler));
}

void AppRouter::post(std::string path, RouteHandler handler) {
  routes_.emplace(key("POST", std::move(path)), std::move(handler));
}

void AppRouter::put(std::string path, RouteHandler handler) {
  routes_.emplace(key("PUT", std::move(path)), std::move(handler));
}

void AppRouter::del(std::string path, RouteHandler handler) {
  routes_.emplace(key("DELETE", std::move(path)), std::move(handler));
}

HttpResponse AppRouter::route(const HttpRequest& request) const {
  const auto it = routes_.find(key(request.method, request.target));
  if (it == routes_.end()) {
    return HttpResponse{404, "application/json", error_body("not found")};
  }

  try {
    return it->second(request);
  } catch (const std::exception& exception) {
    return HttpResponse{500, "application/json", error_body(exception.what())};
  }
}

std::string AppRouter::key(std::string method, std::string path) {
  return std::move(method) + " " + std::move(path);
}
