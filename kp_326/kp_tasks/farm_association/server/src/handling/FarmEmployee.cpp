#include <handling/FarmEmployee.hpp>

#include <handling/RequestParsing.hpp>

#include <controllers/app/FarmEmployee.hpp>

#include <marshalling/FarmEmployee.hpp>
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

fasc::server::controllers::dto::FarmEmployeeKeyDto keyFrom(
    const fasc::server::core::HttpRequest& request) {
  fasc::server::controllers::dto::FarmEmployeeKeyDto key;
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

FarmEmployeeHandler::FarmEmployeeHandler(fasc::server::controllers::http::FarmEmployeeHttpController& controller)
    : controller_(controller) {}

fasc::server::core::HttpResponse FarmEmployeeHandler::list(const fasc::server::core::HttpRequest&) {
  return responseFrom(controller_.list(), 200);
}

fasc::server::core::HttpResponse FarmEmployeeHandler::load(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.load(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmEmployeeHandler::create(
    const fasc::server::core::HttpRequest& request) {
  return responseFrom(controller_.create(request.body), 201);
}

fasc::server::core::HttpResponse FarmEmployeeHandler::update(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.update(keyFrom(request), request.body), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

fasc::server::core::HttpResponse FarmEmployeeHandler::erase(
    const fasc::server::core::HttpRequest& request) {
  try {
    return responseFrom(controller_.erase(keyFrom(request)), 200);
  } catch (const std::exception& exception) {
    return badRequest(exception);
  }
}

void registerFarmEmployeeRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database) {
  auto farmemployeeController = std::make_shared<fasc::server::controllers::app::FarmEmployeeController>(database);
  auto farmemployeeHttpController = std::make_shared<fasc::server::controllers::http::FarmEmployeeHttpController>(*farmemployeeController);
  auto farmemployeeHandler = std::make_shared<FarmEmployeeHandler>(*farmemployeeHttpController);
  server.get("/api/farm_employee", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->list(request); });
  server.post("/api/farm_employee", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->create(request); });
  server.get("/api/farm_employee/item", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->load(request); });
  server.put("/api/farm_employee/item", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->update(request); });
  server.del("/api/farm_employee/item", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->erase(request); });
}

} // namespace fasc::server::handling
