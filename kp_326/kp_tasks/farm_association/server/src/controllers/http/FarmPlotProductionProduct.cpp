#include <controllers/http/FarmPlotProductionProduct.hpp>

#include <marshalling/FarmPlotProductionProduct.hpp>

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

FarmPlotProductionProductHttpController::FarmPlotProductionProductHttpController(fasc::server::controllers::app::FarmPlotProductionProductController& controller)
    : controller_(controller) {}

FarmPlotProductionProductRowsViewResult FarmPlotProductionProductHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmPlotProductionProductRowsViewResult::failure(errorView(result.error()));
  }
  return FarmPlotProductionProductRowsViewResult::success(fasc::server::views::FarmPlotProductionProductRowsView{result.success().rows});
}

FarmPlotProductionProductRowViewResult FarmPlotProductionProductHttpController::load(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmPlotProductionProductRowViewResult::failure(errorView(result.error()));
  }
  return FarmPlotProductionProductRowViewResult::success(fasc::server::views::FarmPlotProductionProductRowView{result.success().data});
}

FarmPlotProductionProductMutationViewResult FarmPlotProductionProductHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotProductionProductCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmPlotProductionProductMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotProductionProductMutationViewResult::success(
        fasc::server::views::FarmPlotProductionProductMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotProductionProductMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotProductionProductMutationViewResult FarmPlotProductionProductHttpController::update(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotProductionProductUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmPlotProductionProductMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotProductionProductMutationViewResult::success(
        fasc::server::views::FarmPlotProductionProductMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotProductionProductMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotProductionProductMutationViewResult FarmPlotProductionProductHttpController::erase(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmPlotProductionProductMutationViewResult::failure(errorView(result.error()));
  }
  return FarmPlotProductionProductMutationViewResult::success(
      fasc::server::views::FarmPlotProductionProductMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
