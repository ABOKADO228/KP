#include <controllers/app/Users.hpp>
#include <controllers/http/Users.hpp>
#include <database/Bootstrap.hpp>
#include <database/Database.hpp>
#include <handling/FarmEntityRoutes.hpp>
#include <handling/Health.hpp>
#include <handling/Users.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>
#include <server/core/Server.hpp>

#include <fmt/core.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {

using fasc::server::controllers::app::UserController;
using fasc::server::controllers::http::UserHttpController;
using fasc::server::core::HttpRequest;
using fasc::server::core::Server;
using fasc::server::core::ServerSettings;
using fasc::server::handling::HealthHandler;
using fasc::server::handling::registerFarmEntityRoutes;
using fasc::server::handling::UserHandler;

constexpr std::string_view kServerName = "farm-association-server";
constexpr std::string_view kServerVersion = "0.1.0";
constexpr std::string_view kDefaultJwtSecret = "development-secret-change-me";

void printVersion() {
  std::cout << fmt::format("{} version {}\n", kServerName, kServerVersion);
}

std::string envOr(const char* name, std::string fallback) {
  if (const char* value = std::getenv(name)) {
    return value;
  }

  return fallback;
}

bool envBoolOr(const char* name, bool fallback) {
  const char* raw = std::getenv(name);
  if (raw == nullptr) {
    return fallback;
  }

  std::string value{raw};
  std::ranges::transform(value, value.begin(), [](unsigned char ch) {
    return static_cast<char>(std::tolower(ch));
  });

  if (value == "1" || value == "true" || value == "yes" || value == "on") {
    return true;
  }
  if (value == "0" || value == "false" || value == "no" || value == "off") {
    return false;
  }

  throw std::invalid_argument{std::string{name} + " must be a boolean value"};
}

template <typename Value>
Value parseNumber(std::string_view value, std::string_view setting_name) {
  if (value.empty() || value.front() == '-') {
    throw std::invalid_argument{std::string{setting_name} + " must be a positive number"};
  }

  std::size_t parsed = 0;
  const auto number = std::stoull(std::string{value}, &parsed);
  if (parsed != value.size()) {
    throw std::invalid_argument{std::string{setting_name} + " must be a number"};
  }

  if (number > static_cast<unsigned long long>(std::numeric_limits<Value>::max())) {
    throw std::out_of_range{std::string{setting_name} + " is out of range"};
  }

  return static_cast<Value>(number);
}

ServerSettings settingsFromEnv() {
  ServerSettings settings;
  settings.address = envOr("FARM_SERVER_ADDRESS", settings.address);

  if (const char* value = std::getenv("FARM_SERVER_PORT")) {
    settings.port = parseNumber<unsigned short>(value, "FARM_SERVER_PORT");
  }

  if (const char* value = std::getenv("FARM_SERVER_THREADS")) {
    settings.thread_count = parseNumber<std::size_t>(value, "FARM_SERVER_THREADS");
  }

  if (const char* value = std::getenv("FARM_SERVER_BODY_LIMIT")) {
    settings.request_body_limit = parseNumber<std::size_t>(value, "FARM_SERVER_BODY_LIMIT");
  }

  if (const char* value = std::getenv("FARM_SERVER_TIMEOUT_SECONDS")) {
    settings.request_timeout =
        std::chrono::seconds{parseNumber<long long>(value, "FARM_SERVER_TIMEOUT_SECONDS")};
  }

  if (const char* value = std::getenv("FARM_SERVER_LISTEN_BACKLOG")) {
    settings.listen_backlog = parseNumber<int>(value, "FARM_SERVER_LISTEN_BACKLOG");
  }

  return settings;
}

void applyCliArg(ServerSettings& settings, std::string_view option, std::string_view value) {
  if (option == "--address") {
    settings.address = value;
  } else if (option == "--port") {
    settings.port = parseNumber<unsigned short>(value, "--port");
  } else if (option == "--threads") {
    settings.thread_count = parseNumber<std::size_t>(value, "--threads");
  } else if (option == "--body-limit") {
    settings.request_body_limit = parseNumber<std::size_t>(value, "--body-limit");
  } else if (option == "--timeout") {
    settings.request_timeout = std::chrono::seconds{parseNumber<long long>(value, "--timeout")};
  } else if (option == "--backlog") {
    settings.listen_backlog = parseNumber<int>(value, "--backlog");
  } else {
    throw std::invalid_argument{std::string{"Unknown option: "} + std::string{option}};
  }
}

ServerSettings settingsFromEnvAndCli(int argc, char* argv[]) {
  ServerSettings settings = settingsFromEnv();

  for (int i = 1; i < argc; ++i) {
    const std::string_view option{argv[i]};
    if (option == "--version") {
      continue;
    }

    if (i + 1 >= argc) {
      throw std::invalid_argument{std::string{"Missing value for option: "} +
                                  std::string{option}};
    }

    applyCliArg(settings, option, argv[++i]);
  }

  return settings;
}

void ensureBuiltinAdministrator(UserController& user_controller) {
  if (!envBoolOr("FARM_ADMIN_ENABLED", true)) {
    return;
  }

  const std::string login = envOr("FARM_ADMIN_LOGIN", "admin");
  const std::string password = envOr("FARM_ADMIN_PASSWORD", "admin12345");
  const auto result = user_controller.createUser(
      fasc::server::controllers::dto::CreateUserCommand{
          login, password, fasc::server::controllers::dto::kAdministratorUserRole});

  if (result.hasError()) {
    if (result.error().code == fasc::server::controllers::app::UserErrorCode::Conflict) {
      std::cout << fmt::format("builtin administrator already exists: {}\n", login);
      return;
    }

    throw std::runtime_error{"Failed to ensure builtin administrator: " +
                             result.error().message};
  }

  std::cout << fmt::format("builtin administrator created: {}\n", login);
}

} // namespace

int main(int argc, char* argv[]) {
  if (argc > 1 && std::string_view{argv[1]} == "--version") {
    printVersion();
    return 0;
  }

  const ServerSettings settings = settingsFromEnvAndCli(argc, argv);

  auto databaseBootstrap = fasc::server::database::bootstrapOptionsFromEnv();
  fasc::server::database::prepareDatabase(databaseBootstrap);
  auto database = fasc::server::database::Database::create(std::move(databaseBootstrap.connection));
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{
      envOr("FARM_JWT_SECRET", std::string{kDefaultJwtSecret})};

  UserController user_controller{database, passwordHasher, jwt_service};
  ensureBuiltinAdministrator(user_controller);
  UserHttpController user_http_controller{user_controller};

  HealthHandler health_handler;
  UserHandler user_handler{user_http_controller, jwt_service};

  Server server;
  server.get("/health", [&](const HttpRequest& request) { return health_handler.health(request); });
  server.post("/auth/register",
              [&](const HttpRequest& request) { return user_handler.registerUser(request); });
  server.post("/auth/login",
              [&](const HttpRequest& request) { return user_handler.loginUser(request); });
  server.post("/users",
              [&](const HttpRequest& request) { return user_handler.createUser(request); });
  registerFarmEntityRoutes(server, database);

  std::cout << fmt::format("{} starting on {}:{}\n", kServerName, settings.address, settings.port);
  return server.run(settings);
}
