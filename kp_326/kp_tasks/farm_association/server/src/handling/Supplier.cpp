#include <handling/Supplier.hpp>

#include <handling/RequestParsing.hpp>

#include <controllers/app/Supplier.hpp>

#include <marshalling/Supplier.hpp>
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

fasc::server::controllers::dto::SupplierKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::SupplierKeyDto key;
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

SupplierHandler::SupplierHandler(fasc::server::controllers::http::SupplierHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse SupplierHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse SupplierHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse SupplierHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse SupplierHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse SupplierHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerSupplierRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto supplierController = std::make_shared<fasc::server::controllers::app::SupplierController>(database);
  auto supplierHttpController = std::make_shared<fasc::server::controllers::http::SupplierHttpController>(*supplierController);
  auto supplierHandler = std::make_shared<SupplierHandler>(*supplierHttpController);
  server.get("/api/supplier", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->list(request); });
  server.post("/api/supplier", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->create(request); });
  server.get("/api/supplier/item", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->load(request); });
  server.put("/api/supplier/item", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->update(request); });
  server.del("/api/supplier/item", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->erase(request); });
}

} // namespace fasc::server::handling
