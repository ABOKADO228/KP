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

} // namespace
