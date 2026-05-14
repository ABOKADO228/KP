#include <controllers/app/AssociationFarms.hpp>
#include <controllers/app/Farm.hpp>
#include <controllers/http/AssociationFarms.hpp>
#include <controllers/http/Farm.hpp>
#include <fixtures/HttpRequestFixture.hpp>
#include <handling/AssociationFarms.hpp>
#include <handling/Farm.hpp>
#include <utils/JsonAssertions.hpp>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

namespace {

using fasc::server::controllers::app::AssociationFarmsController;
using fasc::server::controllers::app::FarmController;
using fasc::server::controllers::http::AssociationFarmsHttpController;
using fasc::server::controllers::http::FarmHttpController;
using fasc::server::core::HttpResponse;
using fasc::server::handling::AssociationFarmsHandler;
using fasc::server::handling::FarmHandler;
using fasc::server::tests::fixtures::makeHttpRequest;
using fasc::server::tests::utils::expectJsonStringField;

TEST(FarmEntityHandlerTests, ReturnsBadRequestForInvalidCreateJson) {
  fasc::server::database::Database database{nullptr};
  FarmController controller{database};
  FarmHttpController httpController{controller};
  FarmHandler handler{httpController};

  const HttpResponse response = handler.create(makeHttpRequest("POST", "/api/farm", "{"));

  EXPECT_EQ(response.status, 400U);
  const auto body = nlohmann::json::parse(response.body);
  EXPECT_NE(body.at("error").get<std::string>().find("parse error"), std::string::npos);
}

TEST(FarmEntityHandlerTests, ReturnsBadRequestForMissingCompositeKey) {
  fasc::server::database::Database database{nullptr};
  AssociationFarmsController controller{database};
  AssociationFarmsHttpController httpController{controller};
  AssociationFarmsHandler handler{httpController};

  const HttpResponse response = handler.load(makeHttpRequest("GET", "/api/association_farms/item"));

  EXPECT_EQ(response.status, 400U);
  expectJsonStringField(response.body, "error", "Missing key field: farm_id");
}

} // namespace
