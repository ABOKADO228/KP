#include <controllers/http/FarmEmployee.hpp>

#include <marshalling/FarmEmployee.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::controllers::http {

namespace {

fasc::server::views::ErrorView errorView(const fasc::server::controllers::app::FarmEntityError& error) {
  using fasc::server::controllers::app::FarmEntityErrorCode;
  using fasc::server::views::ErrorViewCode;
  switch (error.code) {
  case FarmEntityErrorCode::InvalidInput:
    return fasc::server::views::ErrorView{ErrorViewCode::BadRequest, error.message};
  case FarmEntityErrorCode::NotFound:
    return fasc::server::views::ErrorView{ErrorViewCode::NotFound, error.message};
  case FarmEntityErrorCode::PersistenceFailure:
    return fasc::server::views::ErrorView{ErrorViewCode::InternalServerError, error.message};
  }
  return fasc::server::views::ErrorView{ErrorViewCode::InternalServerError, "Unknown data error"};
}

} // namespace

FarmEmployeeHttpController::FarmEmployeeHttpController(fasc::server::controllers::app::FarmEmployeeController& controller)
    : controller_(controller) {}

FarmEmployeeRowsViewResult FarmEmployeeHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmEmployeeRowsViewResult::failure(errorView(result.error()));
  }
  return FarmEmployeeRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmEmployeeRowViewResult FarmEmployeeHttpController::load(
    const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmEmployeeRowViewResult::failure(errorView(result.error()));
  }
  return FarmEmployeeRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmEmployeeMutationViewResult FarmEmployeeHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmEmployeeCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmEmployeeMutationViewResult::failure(errorView(result.error()));
    }
    return FarmEmployeeMutationViewResult::success(
        fasc::server::views::FarmEmployeeMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmEmployeeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmEmployeeMutationViewResult FarmEmployeeHttpController::update(
    const fasc::server::controllers::dto::FarmEmployeeKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmEmployeeUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmEmployeeMutationViewResult::failure(errorView(result.error()));
    }
    return FarmEmployeeMutationViewResult::success(
        fasc::server::views::FarmEmployeeMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmEmployeeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmEmployeeMutationViewResult FarmEmployeeHttpController::erase(
    const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmEmployeeMutationViewResult::failure(errorView(result.error()));
  }
  return FarmEmployeeMutationViewResult::success(
      fasc::server::views::FarmEmployeeMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
