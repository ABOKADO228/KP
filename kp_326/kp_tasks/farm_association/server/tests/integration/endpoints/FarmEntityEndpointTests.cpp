#include <database/Database.hpp>
#include <fixtures/BeastRequestFixture.hpp>
#include <handling/FarmEntityRoutes.hpp>
#include <server/core/RequestDispatcher.hpp>
#include <server/core/Server.hpp>
#include <utils/JsonAssertions.hpp>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

namespace {

namespace http = boost::beast::http;

using fasc::server::core::BeastResponse;
using fasc::server::core::RequestDispatcher;
using fasc::server::core::Server;
using fasc::server::handling::registerFarmEntityRoutes;
using fasc::server::tests::fixtures::makeBeastRequest;
using fasc::server::tests::utils::expectJsonStringField;

TEST(FarmEntityEndpointTests, RegistersNamedRoutesForFarmTables) {
  fasc::server::database::Database database{nullptr};
  Server server;
  registerFarmEntityRoutes(server, database);

  const BeastResponse response = RequestDispatcher{server.routerForTests()}.dispatch(
      makeBeastRequest(http::verb::get, "/api/association_farms/item"));

  EXPECT_EQ(response.result(), http::status::bad_request);
  expectJsonStringField(response.body(), "error", "Missing key field: farm_id");
}

} // namespace
