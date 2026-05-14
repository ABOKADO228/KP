#include <fixtures/BeastRequestFixture.hpp>
#include <handling/Health.hpp>
#include <server/core/AppRouter.hpp>
#include <server/core/RequestDispatcher.hpp>
#include <utils/JsonAssertions.hpp>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

namespace {

namespace http = boost::beast::http;

using fasc::server::tests::fixtures::makeBeastRequest;
using fasc::server::tests::utils::expectJsonStringField;
using fasc::server::core::AppRouter;
using fasc::server::core::BeastResponse;
using fasc::server::core::HttpRequest;
using fasc::server::core::RequestDispatcher;
using fasc::server::handling::HealthHandler;

TEST(HealthEndpointTests, GetHealthReturnsOkThroughHttpPipeline) {
  HealthHandler health_handler;
  AppRouter router;
  router.get("/health", [&](const HttpRequest& request) {
    return health_handler.health(request);
  });
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(makeBeastRequest(http::verb::get, "/health"));

  EXPECT_EQ(response.result(), http::status::ok);
  EXPECT_EQ(response[http::field::content_type], "application/json");
  expectJsonStringField(response.body(), "status", "ok");
}

} // namespace
