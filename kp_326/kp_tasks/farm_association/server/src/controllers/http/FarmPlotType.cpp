#include <controllers/http/FarmPlotType.hpp>

#include <exception>

#include <marshalling/FarmPlotType.hpp>

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

FarmPlotTypeHttpController::FarmPlotTypeHttpController(fasc::server::controllers::app::FarmPlotTypeController& controller)
    : controller_(controller) {}

FarmPlotTypeRowsViewResult FarmPlotTypeHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmPlotTypeRowsViewResult::failure(errorView(result.error()));
  }
  return FarmPlotTypeRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmPlotTypeRowViewResult FarmPlotTypeHttpController::load(
    const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmPlotTypeRowViewResult::failure(errorView(result.error()));
  }
  return FarmPlotTypeRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmPlotTypeMutationViewResult FarmPlotTypeHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotTypeCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmPlotTypeMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotTypeMutationViewResult::success(
        fasc::server::views::FarmPlotTypeMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotTypeMutationViewResult FarmPlotTypeHttpController::update(
    const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotTypeUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmPlotTypeMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotTypeMutationViewResult::success(
        fasc::server::views::FarmPlotTypeMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotTypeMutationViewResult FarmPlotTypeHttpController::erase(
    const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmPlotTypeMutationViewResult::failure(errorView(result.error()));
  }
  return FarmPlotTypeMutationViewResult::success(
      fasc::server::views::FarmPlotTypeMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
