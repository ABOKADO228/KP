#include <handling/FarmOwnership.hpp>

#include <controllers/app/FarmOwnership.hpp>

#include <marshalling/FarmOwnership.hpp>
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

fasc::server::controllers::dto::FarmOwnershipKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::FarmOwnershipKeyDto key;
  if (const auto it = request.query_params.find("id"); it != request.query_params.end()) {
    key.id = std::stoull(it->second);
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

FarmOwnershipHandler::FarmOwnershipHandler(fasc::server::controllers::http::FarmOwnershipHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse FarmOwnershipHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse FarmOwnershipHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmOwnershipHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse FarmOwnershipHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmOwnershipHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerFarmOwnershipRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto farmownershipController = std::make_shared<fasc::server::controllers::app::FarmOwnershipController>(database);
  auto farmownershipHttpController = std::make_shared<fasc::server::controllers::http::FarmOwnershipHttpController>(*farmownershipController);
  auto farmownershipHandler = std::make_shared<FarmOwnershipHandler>(*farmownershipHttpController);
  server.get("/api/farm_ownership", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->list(request); });
  server.post("/api/farm_ownership", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->create(request); });
  server.get("/api/farm_ownership/item", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->load(request); });
  server.put("/api/farm_ownership/item", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->update(request); });
  server.del("/api/farm_ownership/item", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->erase(request); });
}

} // namespace fasc::server::handling
