#include <handling/Unit.hpp>

#include <controllers/app/Unit.hpp>

#include <marshalling/Unit.hpp>
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

fasc::server::controllers::dto::UnitKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::UnitKeyDto key;
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

UnitHandler::UnitHandler(fasc::server::controllers::http::UnitHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse UnitHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse UnitHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse UnitHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse UnitHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse UnitHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerUnitRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto unitController = std::make_shared<fasc::server::controllers::app::UnitController>(database);
  auto unitHttpController = std::make_shared<fasc::server::controllers::http::UnitHttpController>(*unitController);
  auto unitHandler = std::make_shared<UnitHandler>(*unitHttpController);
  server.get("/api/unit", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->list(request); });
  server.post("/api/unit", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->create(request); });
  server.get("/api/unit/item", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->load(request); });
  server.put("/api/unit/item", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->update(request); });
  server.del("/api/unit/item", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->erase(request); });
}

} // namespace fasc::server::handling
