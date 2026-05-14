#include <database/Database.hpp>
#include <fixtures/BeastRequestFixture.hpp>
#include <handling/FarmEntityRoutes.hpp>
#include <server/core/RequestDispatcher.hpp>
#include <server/core/Server.hpp>
#include <utils/JsonAssertions.hpp>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

#include <ostream>
#include <string>
#include <vector>

namespace {

namespace http = boost::beast::http;

using fasc::server::core::BeastResponse;
using fasc::server::core::RequestDispatcher;
using fasc::server::core::Server;
using fasc::server::handling::registerFarmEntityRoutes;
using fasc::server::tests::fixtures::makeBeastRequest;
using fasc::server::tests::utils::expectJsonStringField;

struct MissingKeyRouteCase {
  std::string target;
  std::string expectedError;
};

std::ostream& operator<<(std::ostream& stream, const MissingKeyRouteCase& value) {
  return stream << value.target;
}

class FarmEntityMissingKeyEndpointTests : public testing::TestWithParam<MissingKeyRouteCase> {};

TEST(FarmEntityEndpointTests, RegistersNamedRoutesForFarmTables) {
  fasc::server::database::Database database{nullptr};
  Server server;
  registerFarmEntityRoutes(server, database);

  const BeastResponse response = RequestDispatcher{server.routerForTests()}.dispatch(
      makeBeastRequest(http::verb::get, "/api/association_farms/item"));

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", "Missing key field: farm_id");
}

TEST_P(FarmEntityMissingKeyEndpointTests, RegisteredItemRouteReturnsBadRequestBeforeDatabaseAccess) {
  fasc::server::database::Database database{nullptr};
  Server server;
  registerFarmEntityRoutes(server, database);

  const auto& route = GetParam();
  const BeastResponse response = RequestDispatcher{server.routerForTests()}.dispatch(
      makeBeastRequest(http::verb::get, route.target));

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", route.expectedError);
}

INSTANTIATE_TEST_SUITE_P(
    RegisteredFarmEntityRoutes,
    FarmEntityMissingKeyEndpointTests,
    testing::ValuesIn(std::vector<MissingKeyRouteCase>{
        {"/api/association_employee/item", "Missing key field: id"},
        {"/api/association_farms/item", "Missing key field: farm_id"},
        {"/api/association_member/item", "Missing key field: id"},
        {"/api/association_role/item", "Missing key field: id"},
        {"/api/contract/item", "Missing key field: id"},
        {"/api/employee_plot_assignment/item", "Missing key field: id"},
        {"/api/employment_status/item", "Missing key field: id"},
        {"/api/farm/item", "Missing key field: id"},
        {"/api/farm_association/item", "Missing key field: id"},
        {"/api/farm_employee/item", "Missing key field: id"},
        {"/api/farm_owner/item", "Missing key field: id"},
        {"/api/farm_ownership/item", "Missing key field: id"},
        {"/api/farm_plot/item", "Missing key field: id"},
        {"/api/farm_plot_assignment/item", "Missing key field: farm_id"},
        {"/api/farm_plot_consumption_product/item", "Missing key field: product_id"},
        {"/api/farm_plot_production_product/item", "Missing key field: product_id"},
        {"/api/farm_plot_type/item", "Missing key field: id"},
        {"/api/farm_role/item", "Missing key field: id"},
        {"/api/identity_document_type/item", "Missing key field: id"},
        {"/api/person/item", "Missing key field: id"},
        {"/api/person_document/item", "Missing key field: id"},
        {"/api/product/item", "Missing key field: id"},
        {"/api/product_type/item", "Missing key field: id"},
        {"/api/purchase_order/item", "Missing key field: id"},
        {"/api/purchase_order_item/item", "Missing key field: id"},
        {"/api/purchase_requisition/item", "Missing key field: id"},
        {"/api/sales_requisition/item", "Missing key field: id"},
        {"/api/supplier/item", "Missing key field: id"},
        {"/api/supplier_product_price/item", "Missing key field: id"},
        {"/api/unit/item", "Missing key field: id"},
    }));

} // namespace
