#include <controllers/app/Users.hpp>
#include <controllers/http/Users.hpp>
#include <database/database.hpp>
#include <handling/Health.hpp>
#include <handling/Users.hpp>
#include <server/core/Server.hpp>

#include <fmt/core.h>

#include <iostream>
#include <string_view>

namespace {

constexpr std::string_view kServerName = "farm-association-server";
constexpr std::string_view kServerVersion = "0.1.0";

void print_version() {
  std::cout << fmt::format("{} version {}\n", kServerName, kServerVersion);
}

} // namespace

int main(int argc, char* argv[]) {
  if (argc > 1 && std::string_view{argv[1]} == "--version") {
    print_version();
    return 0;
  }

  auto database = fasc::server::database::Database::createFromEnv();

  UserController user_controller{database};
  UserHttpController user_http_controller{user_controller};

  HealthHandler health_handler;
  UserHandler user_handler{user_http_controller};

  Server server;
  server.get("/health", [&](const HttpRequest& request) { return health_handler.health(request); });
  server.post("/users",
              [&](const HttpRequest& request) { return user_handler.create_user(request); });

  std::cout << fmt::format("{} started on port 8080\n", kServerName);
  return server.run(8080);
}
