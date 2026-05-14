#include <handling/PurchaseOrderItem.hpp>

#include <controllers/app/PurchaseOrderItem.hpp>

#include <marshalling/PurchaseOrderItem.hpp>
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

fasc::server::controllers::dto::PurchaseOrderItemKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::PurchaseOrderItemKeyDto key;
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

PurchaseOrderItemHandler::PurchaseOrderItemHandler(fasc::server::controllers::http::PurchaseOrderItemHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse PurchaseOrderItemHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse PurchaseOrderItemHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse PurchaseOrderItemHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse PurchaseOrderItemHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse PurchaseOrderItemHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerPurchaseOrderItemRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto purchaseorderitemController = std::make_shared<fasc::server::controllers::app::PurchaseOrderItemController>(database);
  auto purchaseorderitemHttpController = std::make_shared<fasc::server::controllers::http::PurchaseOrderItemHttpController>(*purchaseorderitemController);
  auto purchaseorderitemHandler = std::make_shared<PurchaseOrderItemHandler>(*purchaseorderitemHttpController);
  server.get("/api/purchase_order_item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->list(request); });
  server.post("/api/purchase_order_item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->create(request); });
  server.get("/api/purchase_order_item/item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->load(request); });
  server.put("/api/purchase_order_item/item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->update(request); });
  server.del("/api/purchase_order_item/item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->erase(request); });
}

} // namespace fasc::server::handling
