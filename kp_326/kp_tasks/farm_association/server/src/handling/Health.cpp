#include <handling/Health.hpp>

namespace fasc::server::handling {

HttpResponse HealthHandler::health(const HttpRequest&) {
  return HttpResponse{200, "application/json", R"({"status":"ok"})"};
}

} // namespace fasc::server::handling
