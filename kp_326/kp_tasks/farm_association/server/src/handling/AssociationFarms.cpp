#include <handling/AssociationFarms.hpp>

#include <controllers/app/AssociationFarms.hpp>

#include <marshalling/AssociationFarms.hpp>
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

fasc::server::controllers::dto::AssociationFarmsKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::AssociationFarmsKeyDto key;
  if (const auto it = request.query_params.find("farm_id"); it != request.query_params.end()) {
    key.farmId = std::stoull(it->second);
  } else {
    throw std::invalid_argument{"Missing key field: farm_id"};
  }
  if (const auto it = request.query_params.find("association_id"); it != request.query_params.end()) {
    key.associationId = std::stoull(it->second);
  } else {
    throw std::invalid_argument{"Missing key field: association_id"};
  }
  return key;
}

fasc::server::core::HttpResponse badRequest(const std::exception& exception) {
  const fasc::server::views::ErrorView error{fasc::server::views::ErrorViewCode::BadRequest,
                                             exception.what()};
  return fasc::server::core::HttpResponse{400, "application/json", nlohmann::json(error).dump()};
}

} // namespace

AssociationFarmsHandler::AssociationFarmsHandler(fasc::server::controllers::http::AssociationFarmsHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse AssociationFarmsHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse AssociationFarmsHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse AssociationFarmsHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse AssociationFarmsHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse AssociationFarmsHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerAssociationFarmsRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto associationfarmsController = std::make_shared<fasc::server::controllers::app::AssociationFarmsController>(database);
  auto associationfarmsHttpController = std::make_shared<fasc::server::controllers::http::AssociationFarmsHttpController>(*associationfarmsController);
  auto associationfarmsHandler = std::make_shared<AssociationFarmsHandler>(*associationfarmsHttpController);
  server.get("/api/association_farms", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->list(request); });
  server.post("/api/association_farms", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->create(request); });
  server.get("/api/association_farms/item", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->load(request); });
  server.put("/api/association_farms/item", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->update(request); });
  server.del("/api/association_farms/item", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->erase(request); });
}

} // namespace fasc::server::handling
