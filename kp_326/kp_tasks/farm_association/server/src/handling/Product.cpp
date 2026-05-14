#include <handling/Product.hpp>

#include <controllers/app/Product.hpp>

#include <marshalling/Product.hpp>
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

fasc::server::controllers::dto::ProductKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::ProductKeyDto key;
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

ProductHandler::ProductHandler(fasc::server::controllers::http::ProductHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse ProductHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse ProductHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse ProductHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse ProductHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse ProductHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerProductRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto productController = std::make_shared<fasc::server::controllers::app::ProductController>(database);
  auto productHttpController = std::make_shared<fasc::server::controllers::http::ProductHttpController>(*productController);
  auto productHandler = std::make_shared<ProductHandler>(*productHttpController);
  server.get("/api/product", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->list(request); });
  server.post("/api/product", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->create(request); });
  server.get("/api/product/item", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->load(request); });
  server.put("/api/product/item", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->update(request); });
  server.del("/api/product/item", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->erase(request); });
}

} // namespace fasc::server::handling
