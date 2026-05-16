#include <controllers/app/Users.hpp>
#include <controllers/dto/User.hpp>
#include <controllers/http/Users.hpp>
#include <database/Database.hpp>
#include <fixtures/BeastRequestFixture.hpp>
#include <handling/Users.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>
#include <server/core/AppRouter.hpp>
#include <server/core/RequestDispatcher.hpp>
#include <utils/JsonAssertions.hpp>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

namespace {

namespace http = boost::beast::http;

using fasc::server::tests::fixtures::makeBeastRequest;
using fasc::server::tests::utils::expectJsonStringField;
using fasc::server::controllers::app::UserController;
using fasc::server::controllers::dto::UserDto;
using fasc::server::controllers::http::UserHttpController;
using fasc::server::core::AppRouter;
using fasc::server::core::BeastResponse;
using fasc::server::core::HttpRequest;
using fasc::server::core::RequestDispatcher;
using fasc::server::handling::UserHandler;

TEST(UsersBusinessProcessTests, PostUsersWithEmptyLoginReturnsBusinessErrorThroughHttpPipeline) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController user_controller{database, passwordHasher, jwt_service};
  UserHttpController user_http_controller{user_controller};
  UserHandler user_handler{user_http_controller, jwt_service};
  AppRouter router;
  router.post("/users", [&](const HttpRequest& request) {
    return user_handler.createUser(request);
  });
  RequestDispatcher dispatcher{router};

  auto request =
      makeBeastRequest(http::verb::post, "/users", R"({"login":"","password":"password123"})");
  request.set(http::field::authorization,
              "Bearer " + jwt_service.issue(UserDto{
                              "admin",
                              fasc::server::controllers::dto::kAdministratorUserRole,
                          }));

  const BeastResponse response = dispatcher.dispatch(request);

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", "User login is required");
}

TEST(UsersBusinessProcessTests, PostUsersWithoutAuthorizationReturnsUnauthorized) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController user_controller{database, passwordHasher, jwt_service};
  UserHttpController user_http_controller{user_controller};
  UserHandler user_handler{user_http_controller, jwt_service};
  AppRouter router;
  router.post("/users", [&](const HttpRequest& request) {
    return user_handler.createUser(request);
  });
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(makeBeastRequest(http::verb::post,
                                           "/users",
                                           R"({"login":"worker","password":"password123"})"));

  EXPECT_EQ(response.result(), http::status::unauthorized);
  expectJsonStringField(response.body(), "error", "Authorization Bearer token is required");
}

TEST(UsersBusinessProcessTests, PostUsersWithNonAdminTokenReturnsForbidden) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController user_controller{database, passwordHasher, jwt_service};
  UserHttpController user_http_controller{user_controller};
  UserHandler user_handler{user_http_controller, jwt_service};
  AppRouter router;
  router.post("/users", [&](const HttpRequest& request) {
    return user_handler.createUser(request);
  });
  RequestDispatcher dispatcher{router};

  auto request = makeBeastRequest(http::verb::post,
                                  "/users",
                                  R"({"login":"owner","password":"password123","role":"farm_owner"})");
  request.set(http::field::authorization,
              "Bearer " + jwt_service.issue(UserDto{"worker", "farm_worker"}));

  const BeastResponse response = dispatcher.dispatch(request);

  EXPECT_EQ(response.result(), http::status::forbidden);
  expectJsonStringField(response.body(), "error",
                        "Administrator or association director role is required");
}

TEST(UsersBusinessProcessTests, GetUsersWithNonManagerTokenReturnsForbidden) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController user_controller{database, passwordHasher, jwt_service};
  UserHttpController user_http_controller{user_controller};
  UserHandler user_handler{user_http_controller, jwt_service};
  AppRouter router;
  router.get("/users", [&](const HttpRequest& request) {
    return user_handler.listUsers(request);
  });
  RequestDispatcher dispatcher{router};

  auto request = makeBeastRequest(http::verb::get, "/users");
  request.set(http::field::authorization,
              "Bearer " + jwt_service.issue(UserDto{"worker", "farm_worker"}));

  const BeastResponse response = dispatcher.dispatch(request);

  EXPECT_EQ(response.result(), http::status::forbidden);
  expectJsonStringField(response.body(), "error",
                        "Administrator or association director role is required");
}

TEST(UsersBusinessProcessTests, PutUserRoleWithoutLoginQueryReturnsBadRequest) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController user_controller{database, passwordHasher, jwt_service};
  UserHttpController user_http_controller{user_controller};
  UserHandler user_handler{user_http_controller, jwt_service};
  AppRouter router;
  router.put("/users/role", [&](const HttpRequest& request) {
    return user_handler.updateUserRole(request);
  });
  RequestDispatcher dispatcher{router};

  auto request = makeBeastRequest(http::verb::put,
                                  "/users/role",
                                  R"({"role":"agronomist"})");
  request.set(http::field::authorization,
              "Bearer " + jwt_service.issue(UserDto{"director", "association_director"}));

  const BeastResponse response = dispatcher.dispatch(request);

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", "User login query parameter is required");
}

} // namespace
