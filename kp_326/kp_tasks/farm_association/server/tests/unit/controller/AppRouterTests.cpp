#include <fixtures/HttpRequestFixture.hpp>
#include <server/core/AppRouter.hpp>
#include <utils/JsonAssertions.hpp>

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

namespace {

using fasc::server::tests::fixtures::makeHttpRequest;
using fasc::server::tests::utils::expectJsonStringField;
using fasc::server::core::AppRouter;
using fasc::server::core::HttpRequest;
using fasc::server::core::HttpResponse;

TEST(AppRouterTests, RoutesRegisteredGetHandler) {
  AppRouter router;
  router.get("/health", [](const HttpRequest&) {
    return HttpResponse{200, "application/json", R"({"status":"ok"})"};
  });

  const HttpResponse response = router.route(makeHttpRequest("GET", "/health"));

  EXPECT_EQ(response.status, 200U);
  EXPECT_EQ(response.content_type, "application/json");
  expectJsonStringField(response.body, "status", "ok");
}

TEST(AppRouterTests, ReturnsNotFoundForUnknownRoute) {
  const AppRouter router;

  const HttpResponse response = router.route(makeHttpRequest("GET", "/missing"));

  EXPECT_EQ(response.status, 404U);
  expectJsonStringField(response.body, "error", "not found");
}

TEST(AppRouterTests, ConvertsHandlerExceptionToInternalServerError) {
  AppRouter router;
  router.post("/users", [](const HttpRequest&) -> HttpResponse {
    throw std::runtime_error{"failed"};
  });

  const HttpResponse response = router.route(makeHttpRequest("POST", "/users"));

  EXPECT_EQ(response.status, 500U);
  expectJsonStringField(response.body, "error", "failed");
}

TEST(AppRouterTests, EscapesExceptionTextInJsonErrorResponse) {
  AppRouter router;
  router.get("/broken", [](const HttpRequest&) -> HttpResponse {
    throw std::runtime_error{R"(bad "json" text)"};
  });

  const HttpResponse response = router.route(makeHttpRequest("GET", "/broken"));

  EXPECT_EQ(response.status, 500U);
  expectJsonStringField(response.body, "error", R"(bad "json" text)");
}

} // namespace
