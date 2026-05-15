#include <handling/FarmPlotProductionProduct.hpp>

#include <handling/RequestParsing.hpp>

#include <controllers/app/FarmPlotProductionProduct.hpp>

#include <marshalling/FarmPlotProductionProduct.hpp>
#include <marshalling/User.hpp>

#include <nlohmann/json.hpp>

#include <memory>
#include <stdexcept>

namespace fasc::server::handling {

namespace {

unsigned statusFrom(const fasc::server::views::ErrorView& error) {
  using fasc::server::views::ErrorViewCode;
  switch (error.code) {
  case ErrorViewCode::BadRequest:
    return 400;
  case ErrorViewCode::Unauthorized:
    return 401;
  case ErrorViewCode::NotFound:
    return 404;
  case ErrorViewCode::Conflict:
    return 409;
  case ErrorViewCode::InternalServerError:
    return 500;
  }
  return 500;
}

template <typename ResultType>
fasc::server::core::HttpResponse responseFrom(const ResultType& result, unsigned successStatus) {
  if (result.hasError()) {
    return fasc::server::core::HttpResponse{statusFrom(result.error()), "application/json",
                                            nlohmann::json(result.error()).dump()};
  }
  // Успешный view сериализуем в JSON.
  return fasc::server::core::HttpResponse{successStatus, "application/json",
                                          nlohmann::json(result.success()).dump()};
}

fasc::server::controllers::dto::FarmPlotProductionProductKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::FarmPlotProductionProductKeyDto key;
  if (const auto it = request.query_params.find("product_id"); it != request.query_params.end()) {
    key.productId = parseUint64KeyField(it->second, it->first);
  } else {
    throw std::invalid_argument{"Missing key field: product_id"};
  }
  if (const auto it = request.query_params.find("farm_plot_id"); it != request.query_params.end()) {
    key.farmPlotId = parseUint64KeyField(it->second, it->first);
  } else {
    throw std::invalid_argument{"Missing key field: farm_plot_id"};
  }
  return key;
}

fasc::server::core::HttpResponse badRequest(const std::exception& exception) {
  const fasc::server::views::ErrorView error{fasc::server::views::ErrorViewCode::BadRequest,
                                             exception.what()};
  return fasc::server::core::HttpResponse{400, "application/json", nlohmann::json(error).dump()};
}

} // namespace

FarmPlotProductionProductHandler::FarmPlotProductionProductHandler(fasc::server::controllers::http::FarmPlotProductionProductHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse FarmPlotProductionProductHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse FarmPlotProductionProductHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotProductionProductHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse FarmPlotProductionProductHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotProductionProductHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerFarmPlotProductionProductRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto farmplotproductionproductController = std::make_shared<fasc::server::controllers::app::FarmPlotProductionProductController>(database);
  auto farmplotproductionproductHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotProductionProductHttpController>(*farmplotproductionproductController);
  auto farmplotproductionproductHandler = std::make_shared<FarmPlotProductionProductHandler>(*farmplotproductionproductHttpController);
  server.get("/api/farm_plot_production_product", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->list(request); });
  server.post("/api/farm_plot_production_product", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->create(request); });
  server.get("/api/farm_plot_production_product/item", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->load(request); });
  server.put("/api/farm_plot_production_product/item", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->update(request); });
  server.del("/api/farm_plot_production_product/item", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->erase(request); });
}

} // namespace fasc::server::handling
