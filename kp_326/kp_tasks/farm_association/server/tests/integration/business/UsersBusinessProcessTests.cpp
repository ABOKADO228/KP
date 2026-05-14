#include <controllers/app/Users.hpp>
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
using fasc::server::controllers::http::UserHttpController;
using fasc::server::core::AppRouter;
using fasc::server::core::BeastResponse;
using fasc::server::core::HttpRequest;
using fasc::server::core::RequestDispatcher;
using fasc::server::handling::UserHandler;

TEST(UsersBusinessProcessTests, PostUsersWithEmptyNameReturnsBusinessErrorThroughHttpPipeline) {
  fasc::server::database::Database database{nullptr};
  fasc::server::security::PasswordHasher passwordHasher;
  fasc::server::security::JwtService jwt_service{"test-secret"};
  UserController user_controller{database, passwordHasher, jwt_service};
  UserHttpController user_http_controller{user_controller};
  UserHandler user_handler{user_http_controller};
  AppRouter router;
  router.post("/users", [&](const HttpRequest& request) {
    return user_handler.createUser(request);
  });
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(
          makeBeastRequest(http::verb::post, "/users", R"({"name":"","password":"password123"})"));

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", "User name is required");
}

} // namespace
