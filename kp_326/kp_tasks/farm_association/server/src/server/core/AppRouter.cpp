#include <server/core/AppRouter.hpp>

#include <exception>
#include <utility>

void AppRouter::get(std::string path, RouteHandler handler) {
  routes_.emplace(key("GET", std::move(path)), std::move(handler));
}

void AppRouter::post(std::string path, RouteHandler handler) {
  routes_.emplace(key("POST", std::move(path)), std::move(handler));
}

HttpResponse AppRouter::route(const HttpRequest& request) const {
  const auto it = routes_.find(key(request.method, request.target));
  if (it == routes_.end()) {
    return HttpResponse{404, "application/json", R"({"error":"not found"})"};
  }

  try {
    return it->second(request);
  } catch (const std::exception& exception) {
    return HttpResponse{500, "application/json",
                        std::string{R"({"error":")"} + exception.what() + R"("})"};
  }
}

std::string AppRouter::key(std::string method, std::string path) {
  return std::move(method) + " " + std::move(path);
}
