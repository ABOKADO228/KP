#include <controllers/app/Users.hpp>
#include <controllers/http/Users.hpp>
#include <database/Bootstrap.hpp>
#include <database/Database.hpp>
#include <fixtures/BeastRequestFixture.hpp>
#include <handling/FarmEntityRoutes.hpp>
#include <handling/Users.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>
#include <server/core/RequestDispatcher.hpp>
#include <server/core/Server.hpp>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#ifndef FARM_SERVER_DOMAIN_SQL_PATH
#error "FARM_SERVER_DOMAIN_SQL_PATH must be provided by farm_association_server_core"
#endif

#ifndef FARM_SERVER_USER_SQL_PATH
#error "FARM_SERVER_USER_SQL_PATH must be provided by farm_association_server_core"
#endif

namespace {

namespace http = boost::beast::http;

using fasc::server::controllers::app::UserController;
using fasc::server::controllers::http::UserHttpController;
using fasc::server::core::BeastResponse;
using fasc::server::core::HttpRequest;
using fasc::server::core::RequestDispatcher;
using fasc::server::core::Server;
using fasc::server::database::Database;
using fasc::server::database::DatabaseBootstrapMode;
using fasc::server::database::DatabaseBootstrapScript;
using fasc::server::database::DatabaseBootstrapScriptKind;
using fasc::server::database::bootstrapOptionsFromEnv;
using fasc::server::database::prepareDatabase;
using fasc::server::handling::UserHandler;
using fasc::server::handling::registerFarmEntityRoutes;
using fasc::server::tests::fixtures::makeBeastRequest;

constexpr const char* kIntegrationDatabaseName = "fasc_test";

Database createResetIntegrationDatabase() {
  auto options = bootstrapOptionsFromEnv();
  options.enabled = true;
  options.connection.databaseName = kIntegrationDatabaseName;
  options.mode = DatabaseBootstrapMode::ResetAndInitialize;
  options.initializationScripts = std::vector<DatabaseBootstrapScript>{
      {DatabaseBootstrapScriptKind::DomainSchema,
       std::filesystem::path{FARM_SERVER_DOMAIN_SQL_PATH}},
      {DatabaseBootstrapScriptKind::UserSchema,
       std::filesystem::path{FARM_SERVER_USER_SQL_PATH}},
  };

  prepareDatabase(options);
  return Database::create(std::move(options.connection));
}

BeastResponse dispatchFarmEntityRequest(Database& database,
                                        http::verb method,
                                        std::string target,
                                        std::string body = {}) {
  Server server;
  registerFarmEntityRoutes(server, database);
  return RequestDispatcher{server.routerForTests()}.dispatch(
      makeBeastRequest(method, std::move(target), std::move(body)));
}

BeastResponse dispatchAuthRequest(Database& database,
                                  http::verb method,
                                  std::string target,
                                  std::string body) {
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwtService{"integration-test-secret"};
  UserController userController{database, passwordHasher, jwtService};
  UserHttpController userHttpController{userController};
  UserHandler userHandler{userHttpController};

  fasc::server::core::AppRouter router;
  router.post("/auth/register",
              [&](const HttpRequest& request) { return userHandler.registerUser(request); });
  router.post("/auth/login",
              [&](const HttpRequest& request) { return userHandler.loginUser(request); });

  return RequestDispatcher{router}.dispatch(
      makeBeastRequest(method, std::move(target), std::move(body)));
}

} // namespace

TEST(DatabaseBackedEndpointTests, FarmListReadsSeedDataFromFascTest) {
  auto database = createResetIntegrationDatabase();

  const BeastResponse response =
      dispatchFarmEntityRequest(database, http::verb::get, "/api/farm");

  ASSERT_EQ(response.result(), http::status::ok) << response.body();
  const auto body = nlohmann::json::parse(response.body());
  EXPECT_EQ(body.at("resource"), "farm");
  ASSERT_TRUE(body.at("rows").is_array());
  EXPECT_GE(body.at("rows").size(), 1U);
  EXPECT_TRUE(body.at("rows").front().contains("id"));
}

TEST(DatabaseBackedEndpointTests, AuthRegisterAndLoginRoundTripThroughFascTest) {
  auto database = createResetIntegrationDatabase();
  const std::string credentials = R"({"name":"db_integration_user","password":"password123"})";

  const BeastResponse registerResponse =
      dispatchAuthRequest(database, http::verb::post, "/auth/register", credentials);
  ASSERT_EQ(registerResponse.result(), http::status::created) << registerResponse.body();
  const auto registerBody = nlohmann::json::parse(registerResponse.body());
  EXPECT_EQ(registerBody.at("token_type"), "Bearer");
  EXPECT_EQ(registerBody.at("user").at("name"), "db_integration_user");
  EXPECT_GE(registerBody.at("user").at("id").get<std::uint64_t>(), 1U);

  const BeastResponse loginResponse =
      dispatchAuthRequest(database, http::verb::post, "/auth/login", credentials);
  ASSERT_EQ(loginResponse.result(), http::status::ok) << loginResponse.body();
  const auto loginBody = nlohmann::json::parse(loginResponse.body());
  EXPECT_EQ(loginBody.at("token_type"), "Bearer");
  EXPECT_EQ(loginBody.at("user").at("name"), "db_integration_user");
  EXPECT_EQ(loginBody.at("user").at("id"), registerBody.at("user").at("id"));
  const auto token = loginBody.at("token").get<std::string>();
  EXPECT_EQ(std::count(token.begin(), token.end(), '.'), 2);
}
