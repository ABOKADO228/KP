#include <handling/FarmPlotType.hpp>

#include <controllers/app/FarmPlotType.hpp>

#include <marshalling/FarmPlotType.hpp>
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

fasc::server::controllers::dto::FarmPlotTypeKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::FarmPlotTypeKeyDto key;
  if (const auto it = request.query_params.find("id"); it != request.query_params.end()) {
    key.id = std::stoi(it->second);
  } else {
    throw std::invalid_argument{"Missing key field: id"};
  }
  return key;
}

fasc::server::core::HttpResponse badRequest(const std::exception& exception) {
  const fasc::server::views::ErrorView error{fasc::server::views::ErrorViewCode::BadRequest,
                                             exception.what()};
  return fasc::server::core::HttpResponse{400, "application/json", nlohmann::json(error).dump()};
}

} // namespace

FarmPlotTypeHandler::FarmPlotTypeHandler(fasc::server::controllers::http::FarmPlotTypeHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse FarmPlotTypeHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse FarmPlotTypeHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotTypeHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse FarmPlotTypeHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotTypeHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerFarmPlotTypeRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto farmplottypeController = std::make_shared<fasc::server::controllers::app::FarmPlotTypeController>(database);
  auto farmplottypeHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotTypeHttpController>(*farmplottypeController);
  auto farmplottypeHandler = std::make_shared<FarmPlotTypeHandler>(*farmplottypeHttpController);
  server.get("/api/farm_plot_type", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->list(request); });
  server.post("/api/farm_plot_type", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->create(request); });
  server.get("/api/farm_plot_type/item", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->load(request); });
  server.put("/api/farm_plot_type/item", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->update(request); });
  server.del("/api/farm_plot_type/item", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->erase(request); });
}

} // namespace fasc::server::handling
