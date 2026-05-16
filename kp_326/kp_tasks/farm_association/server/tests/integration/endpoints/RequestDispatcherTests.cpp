#include <fixtures/BeastRequestFixture.hpp>
#include <server/core/AppRouter.hpp>
#include <server/core/RequestDispatcher.hpp>
#include <utils/JsonAssertions.hpp>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

#include <string>

namespace {

namespace http = boost::beast::http;

using fasc::server::tests::fixtures::makeBeastRequest;
using fasc::server::tests::utils::expectJsonStringField;
using fasc::server::core::AppRouter;
using fasc::server::core::BeastResponse;
using fasc::server::core::HttpRequest;
using fasc::server::core::HttpResponse;
using fasc::server::core::RequestDispatcher;

TEST(RequestDispatcherIntegrationTests, DispatchesBeastRequestThroughAppRouter) {
  AppRouter router;
  router.post("/echo", [](const HttpRequest& request) {
    return HttpResponse{201, "application/json",
                        std::string{R"({"received":")"} + request.body + R"("})"};
  });
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(makeBeastRequest(http::verb::post, "/echo", "hello"));

  EXPECT_EQ(response.result(), http::status::created);
  EXPECT_EQ(response[http::field::server], "farm-association-server");
  EXPECT_EQ(response[http::field::content_type], "application/json");
  expectJsonStringField(response.body(), "received", "hello");
}

TEST(RequestDispatcherIntegrationTests, ReturnsNotFoundResponseForUnknownRoute) {
  AppRouter router;
  RequestDispatcher dispatcher{router};

  const BeastResponse response = dispatcher.dispatch(makeBeastRequest(http::verb::get, "/missing"));

  EXPECT_EQ(response.result(), http::status::not_found);
  expectJsonStringField(response.body(), "error", "not found");
}

TEST(RequestDispatcherIntegrationTests, RoutesByPathWithoutQueryString) {
  AppRouter router;
  router.get("/health", [](const HttpRequest& request) {
    return HttpResponse{200, "application/json",
                        std::string{R"({"target":")"} + request.target + R"("})"};
  });
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(makeBeastRequest(http::verb::get, "/health?verbose=true"));

  EXPECT_EQ(response.result(), http::status::ok);
  expectJsonStringField(response.body(), "target", "/health");
}

TEST(RequestDispatcherIntegrationTests, AddsSecurityHeadersToResponses) {
  AppRouter router;
  router.get("/health", [](const HttpRequest&) {
    return HttpResponse{200, "application/json", R"({"status":"ok"})"};
  });
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(makeBeastRequest(http::verb::get, "/health"));

  EXPECT_EQ(response["X-Content-Type-Options"], "nosniff");
}

TEST(RequestDispatcherIntegrationTests, AddsCorsHeadersToResponses) {
  AppRouter router;
  router.get("/health", [](const HttpRequest&) {
    return HttpResponse{200, "application/json", R"({"status":"ok"})"};
  });
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(makeBeastRequest(http::verb::get, "/health"));

  EXPECT_EQ(response["Access-Control-Allow-Origin"], "*");
  EXPECT_EQ(response["Access-Control-Allow-Methods"], "GET, POST, PUT, DELETE, OPTIONS");
  EXPECT_EQ(response["Access-Control-Allow-Headers"], "Content-Type, Authorization");
}

TEST(RequestDispatcherIntegrationTests, HandlesCorsPreflightBeforeRouting) {
  AppRouter router;
  RequestDispatcher dispatcher{router};

  const BeastResponse response =
      dispatcher.dispatch(makeBeastRequest(http::verb::options, "/auth/login"));

  EXPECT_EQ(response.result(), http::status::no_content);
  EXPECT_EQ(response["Access-Control-Allow-Origin"], "*");
  EXPECT_EQ(response.body(), "");
}

} // namespace
