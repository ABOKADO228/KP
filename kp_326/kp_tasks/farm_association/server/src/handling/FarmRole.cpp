#include <handling/FarmRole.hpp>

#include <handling/RequestParsing.hpp>

#include <controllers/app/FarmRole.hpp>

#include <marshalling/FarmRole.hpp>
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

fasc::server::controllers::dto::FarmRoleKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::FarmRoleKeyDto key;
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

FarmRoleHandler::FarmRoleHandler(fasc::server::controllers::http::FarmRoleHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse FarmRoleHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse FarmRoleHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmRoleHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse FarmRoleHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmRoleHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerFarmRoleRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto farmroleController = std::make_shared<fasc::server::controllers::app::FarmRoleController>(database);
  auto farmroleHttpController = std::make_shared<fasc::server::controllers::http::FarmRoleHttpController>(*farmroleController);
  auto farmroleHandler = std::make_shared<FarmRoleHandler>(*farmroleHttpController);
  server.get("/api/farm_role", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->list(request); });
  server.post("/api/farm_role", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->create(request); });
  server.get("/api/farm_role/item", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->load(request); });
  server.put("/api/farm_role/item", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->update(request); });
  server.del("/api/farm_role/item", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->erase(request); });
}

} // namespace fasc::server::handling
