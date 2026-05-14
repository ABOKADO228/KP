#include <controllers/http/FarmPlotAssignment.hpp>

#include <marshalling/FarmPlotAssignment.hpp>

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

FarmPlotAssignmentHttpController::FarmPlotAssignmentHttpController(fasc::server::controllers::app::FarmPlotAssignmentController& controller)
    : controller_(controller) {}

FarmPlotAssignmentRowsViewResult FarmPlotAssignmentHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmPlotAssignmentRowsViewResult::failure(errorView(result.error()));
  }
  return FarmPlotAssignmentRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmPlotAssignmentRowViewResult FarmPlotAssignmentHttpController::load(
    const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmPlotAssignmentRowViewResult::failure(errorView(result.error()));
  }
  return FarmPlotAssignmentRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmPlotAssignmentMutationViewResult FarmPlotAssignmentHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotAssignmentCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmPlotAssignmentMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotAssignmentMutationViewResult::success(
        fasc::server::views::FarmPlotAssignmentMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotAssignmentMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotAssignmentMutationViewResult FarmPlotAssignmentHttpController::update(
    const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotAssignmentUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmPlotAssignmentMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotAssignmentMutationViewResult::success(
        fasc::server::views::FarmPlotAssignmentMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotAssignmentMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotAssignmentMutationViewResult FarmPlotAssignmentHttpController::erase(
    const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmPlotAssignmentMutationViewResult::failure(errorView(result.error()));
  }
  return FarmPlotAssignmentMutationViewResult::success(
      fasc::server::views::FarmPlotAssignmentMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
