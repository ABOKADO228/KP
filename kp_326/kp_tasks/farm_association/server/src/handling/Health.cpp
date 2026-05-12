#include <handling/Health.hpp>

HttpResponse HealthHandler::health(const HttpRequest&) {
  return HttpResponse{200, "application/json", R"({"status":"ok"})"};
}
