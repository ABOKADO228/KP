#include <handling/FarmPlot.hpp>

#include <handling/RequestParsing.hpp>

#include <controllers/app/FarmPlot.hpp>

#include <marshalling/FarmPlot.hpp>
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

fasc::server::controllers::dto::FarmPlotKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::FarmPlotKeyDto key;
  if (const auto it = request.query_params.find("id"); it != request.query_params.end()) {
    key.id = parseUint64KeyField(it->second, it->first);
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

FarmPlotHandler::FarmPlotHandler(fasc::server::controllers::http::FarmPlotHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse FarmPlotHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse FarmPlotHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse FarmPlotHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmPlotHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerFarmPlotRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto farmplotController = std::make_shared<fasc::server::controllers::app::FarmPlotController>(database);
  auto farmplotHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotHttpController>(*farmplotController);
  auto farmplotHandler = std::make_shared<FarmPlotHandler>(*farmplotHttpController);
  server.get("/api/farm_plot", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->list(request); });
  server.post("/api/farm_plot", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->create(request); });
  server.get("/api/farm_plot/item", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->load(request); });
  server.put("/api/farm_plot/item", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->update(request); });
  server.del("/api/farm_plot/item", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->erase(request); });
}

} // namespace fasc::server::handling
