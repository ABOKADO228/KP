#include <handling/PurchaseRequisition.hpp>

#include <handling/RequestParsing.hpp>

#include <controllers/app/PurchaseRequisition.hpp>

#include <marshalling/PurchaseRequisition.hpp>
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

fasc::server::controllers::dto::PurchaseRequisitionKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::PurchaseRequisitionKeyDto key;
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

PurchaseRequisitionHandler::PurchaseRequisitionHandler(fasc::server::controllers::http::PurchaseRequisitionHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse PurchaseRequisitionHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse PurchaseRequisitionHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse PurchaseRequisitionHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse PurchaseRequisitionHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse PurchaseRequisitionHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerPurchaseRequisitionRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto purchaserequisitionController = std::make_shared<fasc::server::controllers::app::PurchaseRequisitionController>(database);
  auto purchaserequisitionHttpController = std::make_shared<fasc::server::controllers::http::PurchaseRequisitionHttpController>(*purchaserequisitionController);
  auto purchaserequisitionHandler = std::make_shared<PurchaseRequisitionHandler>(*purchaserequisitionHttpController);
  server.get("/api/purchase_requisition", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->list(request); });
  server.post("/api/purchase_requisition", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->create(request); });
  server.get("/api/purchase_requisition/item", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->load(request); });
  server.put("/api/purchase_requisition/item", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->update(request); });
  server.del("/api/purchase_requisition/item", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->erase(request); });
}

} // namespace fasc::server::handling
