#include <controllers/http/EmploymentStatus.hpp>

#include <exception>

#include <marshalling/EmploymentStatus.hpp>

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

EmploymentStatusHttpController::EmploymentStatusHttpController(fasc::server::controllers::app::EmploymentStatusController& controller)
    : controller_(controller) {}

EmploymentStatusRowsViewResult EmploymentStatusHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return EmploymentStatusRowsViewResult::failure(errorView(result.error()));
  }
  return EmploymentStatusRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

EmploymentStatusRowViewResult EmploymentStatusHttpController::load(
    const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return EmploymentStatusRowViewResult::failure(errorView(result.error()));
  }
  return EmploymentStatusRowViewResult::success(fasc::server::views::toView(result.success().data));
}

EmploymentStatusMutationViewResult EmploymentStatusHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::EmploymentStatusCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return EmploymentStatusMutationViewResult::failure(errorView(result.error()));
    }
    return EmploymentStatusMutationViewResult::success(
        fasc::server::views::EmploymentStatusMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

EmploymentStatusMutationViewResult EmploymentStatusHttpController::update(
    const fasc::server::controllers::dto::EmploymentStatusKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::EmploymentStatusUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return EmploymentStatusMutationViewResult::failure(errorView(result.error()));
    }
    return EmploymentStatusMutationViewResult::success(
        fasc::server::views::EmploymentStatusMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

EmploymentStatusMutationViewResult EmploymentStatusHttpController::erase(
    const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return EmploymentStatusMutationViewResult::failure(errorView(result.error()));
  }
  return EmploymentStatusMutationViewResult::success(
      fasc::server::views::EmploymentStatusMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
