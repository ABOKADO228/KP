#include <controllers/http/EmployeePlotAssignment.hpp>

#include <marshalling/EmployeePlotAssignment.hpp>

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

EmployeePlotAssignmentHttpController::EmployeePlotAssignmentHttpController(fasc::server::controllers::app::EmployeePlotAssignmentController& controller)
    : controller_(controller) {}

EmployeePlotAssignmentRowsViewResult EmployeePlotAssignmentHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return EmployeePlotAssignmentRowsViewResult::failure(errorView(result.error()));
  }
  return EmployeePlotAssignmentRowsViewResult::success(fasc::server::views::EmployeePlotAssignmentRowsView{result.success().rows});
}

EmployeePlotAssignmentRowViewResult EmployeePlotAssignmentHttpController::load(
    const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return EmployeePlotAssignmentRowViewResult::failure(errorView(result.error()));
  }
  return EmployeePlotAssignmentRowViewResult::success(fasc::server::views::EmployeePlotAssignmentRowView{result.success().data});
}

EmployeePlotAssignmentMutationViewResult EmployeePlotAssignmentHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::EmployeePlotAssignmentCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return EmployeePlotAssignmentMutationViewResult::failure(errorView(result.error()));
    }
    return EmployeePlotAssignmentMutationViewResult::success(
        fasc::server::views::EmployeePlotAssignmentMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return EmployeePlotAssignmentMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

EmployeePlotAssignmentMutationViewResult EmployeePlotAssignmentHttpController::update(
    const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::EmployeePlotAssignmentUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return EmployeePlotAssignmentMutationViewResult::failure(errorView(result.error()));
    }
    return EmployeePlotAssignmentMutationViewResult::success(
        fasc::server::views::EmployeePlotAssignmentMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return EmployeePlotAssignmentMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

EmployeePlotAssignmentMutationViewResult EmployeePlotAssignmentHttpController::erase(
    const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return EmployeePlotAssignmentMutationViewResult::failure(errorView(result.error()));
  }
  return EmployeePlotAssignmentMutationViewResult::success(
      fasc::server::views::EmployeePlotAssignmentMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
