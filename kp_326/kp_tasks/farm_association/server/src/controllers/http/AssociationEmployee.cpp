#include <controllers/http/AssociationEmployee.hpp>

#include <exception>

#include <marshalling/AssociationEmployee.hpp>

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

AssociationEmployeeHttpController::AssociationEmployeeHttpController(fasc::server::controllers::app::AssociationEmployeeController& controller)
    : controller_(controller) {}

AssociationEmployeeRowsViewResult AssociationEmployeeHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return AssociationEmployeeRowsViewResult::failure(errorView(result.error()));
  }
  return AssociationEmployeeRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

AssociationEmployeeRowViewResult AssociationEmployeeHttpController::load(
    const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return AssociationEmployeeRowViewResult::failure(errorView(result.error()));
  }
  return AssociationEmployeeRowViewResult::success(fasc::server::views::toView(result.success().data));
}

AssociationEmployeeMutationViewResult AssociationEmployeeHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationEmployeeCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return AssociationEmployeeMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationEmployeeMutationViewResult::success(
        fasc::server::views::AssociationEmployeeMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationEmployeeMutationViewResult AssociationEmployeeHttpController::update(
    const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationEmployeeUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return AssociationEmployeeMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationEmployeeMutationViewResult::success(
        fasc::server::views::AssociationEmployeeMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationEmployeeMutationViewResult AssociationEmployeeHttpController::erase(
    const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return AssociationEmployeeMutationViewResult::failure(errorView(result.error()));
  }
  return AssociationEmployeeMutationViewResult::success(
      fasc::server::views::AssociationEmployeeMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
