#include <handling/FarmPlotConsumptionProduct.hpp>

#include <controllers/app/FarmPlotConsumptionProduct.hpp>

#include <marshalling/FarmPlotConsumptionProduct.hpp>
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

fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto key;
  if (const auto it = request.query_params.find("product_id"); it != request.query_params.end()) {
    key.productId = std::stoull(it->second);
  } else {
    throw std::invalid_argument{"Missing key field: product_id"};
  }
  if (const auto it = request.query_params.find("farm_plot_id"); it != request.query_params.end()) {
    key.farmPlotId = std::stoull(it->second);
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

FarmPlotConsumptionProductHandler::FarmPlotConsumptionProductHandler(fasc::server::controllers::http::FarmPlotConsumptionProductHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse FarmPlotConsumptionProductHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse FarmPlotConsumptionProductHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotConsumptionProductHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse FarmPlotConsumptionProductHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotConsumptionProductHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerFarmPlotConsumptionProductRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto farmplotconsumptionproductController = std::make_shared<fasc::server::controllers::app::FarmPlotConsumptionProductController>(database);
  auto farmplotconsumptionproductHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotConsumptionProductHttpController>(*farmplotconsumptionproductController);
  auto farmplotconsumptionproductHandler = std::make_shared<FarmPlotConsumptionProductHandler>(*farmplotconsumptionproductHttpController);
  server.get("/api/farm_plot_consumption_product", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->list(request); });
  server.post("/api/farm_plot_consumption_product", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->create(request); });
  server.get("/api/farm_plot_consumption_product/item", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->load(request); });
  server.put("/api/farm_plot_consumption_product/item", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->update(request); });
  server.del("/api/farm_plot_consumption_product/item", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->erase(request); });
}

} // namespace fasc::server::handling
