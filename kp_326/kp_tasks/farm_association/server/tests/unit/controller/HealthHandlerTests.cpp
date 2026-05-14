#include <fixtures/HttpRequestFixture.hpp>
#include <handling/Health.hpp>
#include <utils/JsonAssertions.hpp>

#include <gtest/gtest.h>

namespace {

using fasc::server::tests::fixtures::makeHttpRequest;
using fasc::server::tests::utils::expectJsonStringField;
using fasc::server::core::HttpResponse;
using fasc::server::handling::HealthHandler;

TEST(HealthHandlerTests, ReturnsOkStatus) {
  HealthHandler handler;

  const HttpResponse response = handler.health(makeHttpRequest("GET", "/health"));

  EXPECT_EQ(response.status, 200U);
  EXPECT_EQ(response.content_type, "application/json");
  expectJsonStringField(response.body, "status", "ok");
}

} // namespace
