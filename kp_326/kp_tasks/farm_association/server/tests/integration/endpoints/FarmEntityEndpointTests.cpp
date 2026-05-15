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

struct InvalidKeyRouteCase {
  std::string target;
  std::string expectedError;
};

struct RequestCase {
  http::verb method;
  std::string target;
  std::string body;
  unsigned expectedStatus{};
  std::string expectedError;
};

std::ostream& operator<<(std::ostream& stream, const MissingKeyRouteCase& value) {
  return stream << value.target;
}

std::ostream& operator<<(std::ostream& stream, const InvalidKeyRouteCase& value) {
  return stream << value.target;
}

std::ostream& operator<<(std::ostream& stream, const RequestCase& value) {
  return stream << value.target << " " << value.body;
}

class FarmEntityMissingKeyEndpointTests : public testing::TestWithParam<MissingKeyRouteCase> {};
class FarmEntityInvalidKeyEndpointTests : public testing::TestWithParam<InvalidKeyRouteCase> {};
class FarmEntityBadRequestEndpointTests : public testing::TestWithParam<RequestCase> {};

BeastResponse dispatchFarmEntityRequest(http::verb method,
                                        std::string target,
                                        std::string body = {}) {
  fasc::server::database::Database database{nullptr};
  Server server;
  registerFarmEntityRoutes(server, database);
  return RequestDispatcher{server.routerForTests()}.dispatch(
      makeBeastRequest(method, std::move(target), std::move(body)));
}

TEST(FarmEntityEndpointTests, RegistersNamedRoutesForFarmTables) {
  const BeastResponse response =
      dispatchFarmEntityRequest(http::verb::get, "/api/association_farms/item");

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", "Missing key field: farm_id");
}

TEST_P(FarmEntityMissingKeyEndpointTests, RegisteredItemRouteReturnsBadRequestBeforeDatabaseAccess) {
  const auto& route = GetParam();
  const BeastResponse response = dispatchFarmEntityRequest(http::verb::get, route.target);

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", route.expectedError);
}

TEST_P(FarmEntityInvalidKeyEndpointTests, RegisteredItemRouteRejectsNonNumericKey) {
  const auto& route = GetParam();
  const BeastResponse response = dispatchFarmEntityRequest(http::verb::get, route.target);

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", route.expectedError);
}

TEST_P(FarmEntityBadRequestEndpointTests, RegisteredRouteReturnsExpectedBadRequest) {
  const auto& route = GetParam();
  const BeastResponse response =
      dispatchFarmEntityRequest(route.method, route.target, route.body);

  EXPECT_EQ(response.result_int(), route.expectedStatus);
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

INSTANTIATE_TEST_SUITE_P(
    RegisteredFarmEntityRoutes,
    FarmEntityInvalidKeyEndpointTests,
    testing::ValuesIn(std::vector<InvalidKeyRouteCase>{
        {"/api/association_employee/item?id=1abc", "Invalid key field: id"},
        {"/api/association_farms/item?farm_id=1abc&association_id=1",
         "Invalid key field: farm_id"},
        {"/api/association_member/item?id=1abc", "Invalid key field: id"},
        {"/api/association_role/item?id=1abc", "Invalid key field: id"},
        {"/api/contract/item?id=1abc", "Invalid key field: id"},
        {"/api/employee_plot_assignment/item?id=1abc", "Invalid key field: id"},
        {"/api/employment_status/item?id=1abc", "Invalid key field: id"},
        {"/api/farm/item?id=1abc", "Invalid key field: id"},
        {"/api/farm_association/item?id=1abc", "Invalid key field: id"},
        {"/api/farm_employee/item?id=1abc", "Invalid key field: id"},
        {"/api/farm_owner/item?id=1abc", "Invalid key field: id"},
        {"/api/farm_ownership/item?id=1abc", "Invalid key field: id"},
        {"/api/farm_plot/item?id=1abc", "Invalid key field: id"},
        {"/api/farm_plot_assignment/item?farm_id=1&farm_plot_id=1abc",
         "Invalid key field: farm_plot_id"},
        {"/api/farm_plot_consumption_product/item?product_id=1abc&farm_plot_id=1",
         "Invalid key field: product_id"},
        {"/api/farm_plot_production_product/item?product_id=1&farm_plot_id=1abc",
         "Invalid key field: farm_plot_id"},
        {"/api/farm_plot_type/item?id=1abc", "Invalid key field: id"},
        {"/api/farm_role/item?id=1abc", "Invalid key field: id"},
        {"/api/identity_document_type/item?id=1abc", "Invalid key field: id"},
        {"/api/person/item?id=1abc", "Invalid key field: id"},
        {"/api/person_document/item?id=1abc", "Invalid key field: id"},
        {"/api/product/item?id=1abc", "Invalid key field: id"},
        {"/api/product_type/item?id=1abc", "Invalid key field: id"},
        {"/api/purchase_order/item?id=1abc", "Invalid key field: id"},
        {"/api/purchase_order_item/item?id=1abc", "Invalid key field: id"},
        {"/api/purchase_requisition/item?id=1abc", "Invalid key field: id"},
        {"/api/sales_requisition/item?id=1abc", "Invalid key field: id"},
        {"/api/supplier/item?id=1abc", "Invalid key field: id"},
        {"/api/supplier_product_price/item?id=1abc", "Invalid key field: id"},
        {"/api/unit/item?id=1abc", "Invalid key field: id"},
    }));

INSTANTIATE_TEST_SUITE_P(
    RegisteredFarmEntityRoutes,
    FarmEntityBadRequestEndpointTests,
    testing::ValuesIn(std::vector<RequestCase>{
        {http::verb::get, "/api/association_farms/item?farm_id=1", {}, 400,
         "Missing key field: association_id"},
        {http::verb::get, "/api/farm_plot_assignment/item?farm_id=1", {}, 400,
         "Missing key field: farm_plot_id"},
        {http::verb::get, "/api/farm_plot_consumption_product/item?product_id=1", {}, 400,
         "Missing key field: farm_plot_id"},
        {http::verb::get, "/api/farm_plot_production_product/item?product_id=1", {}, 400,
         "Missing key field: farm_plot_id"},
        {http::verb::post, "/api/farm", "{}", 400, "No writable columns provided"},
        {http::verb::post, "/api/farm_employee", "{}", 400, "Missing field: person_id"},
        {http::verb::put, "/api/farm/item?id=1", "{}", 400, "No writable columns provided"},
        {http::verb::post, "/api/farm", R"({"status":"enabled"})", 400,
         "Unknown enum value: enabled"},
        {http::verb::post, "/api/purchase_order", R"({"order_date":"2026-02-30"})", 400,
         "Invalid date: 2026-02-30"},
    }));

} // namespace
