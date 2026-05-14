#include <controllers/http/FarmPlot.hpp>

#include <marshalling/FarmPlot.hpp>

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

FarmPlotHttpController::FarmPlotHttpController(fasc::server::controllers::app::FarmPlotController& controller)
    : controller_(controller) {}

FarmPlotRowsViewResult FarmPlotHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmPlotRowsViewResult::failure(errorView(result.error()));
  }
  return FarmPlotRowsViewResult::success(fasc::server::views::FarmPlotRowsView{result.success().rows});
}

FarmPlotRowViewResult FarmPlotHttpController::load(
    const fasc::server::controllers::dto::FarmPlotKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmPlotRowViewResult::failure(errorView(result.error()));
  }
  return FarmPlotRowViewResult::success(fasc::server::views::FarmPlotRowView{result.success().data});
}

FarmPlotMutationViewResult FarmPlotHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmPlotMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotMutationViewResult::success(
        fasc::server::views::FarmPlotMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotMutationViewResult FarmPlotHttpController::update(
    const fasc::server::controllers::dto::FarmPlotKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmPlotMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotMutationViewResult::success(
        fasc::server::views::FarmPlotMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotMutationViewResult FarmPlotHttpController::erase(
    const fasc::server::controllers::dto::FarmPlotKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmPlotMutationViewResult::failure(errorView(result.error()));
  }
  return FarmPlotMutationViewResult::success(
      fasc::server::views::FarmPlotMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
