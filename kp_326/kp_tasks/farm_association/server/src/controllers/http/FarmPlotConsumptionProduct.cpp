#include <controllers/http/FarmPlotConsumptionProduct.hpp>

#include <marshalling/FarmPlotConsumptionProduct.hpp>

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

FarmPlotConsumptionProductHttpController::FarmPlotConsumptionProductHttpController(fasc::server::controllers::app::FarmPlotConsumptionProductController& controller)
    : controller_(controller) {}

FarmPlotConsumptionProductRowsViewResult FarmPlotConsumptionProductHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmPlotConsumptionProductRowsViewResult::failure(errorView(result.error()));
  }
  return FarmPlotConsumptionProductRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmPlotConsumptionProductRowViewResult FarmPlotConsumptionProductHttpController::load(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmPlotConsumptionProductRowViewResult::failure(errorView(result.error()));
  }
  return FarmPlotConsumptionProductRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmPlotConsumptionProductMutationViewResult FarmPlotConsumptionProductHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotConsumptionProductCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmPlotConsumptionProductMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotConsumptionProductMutationViewResult::success(
        fasc::server::views::FarmPlotConsumptionProductMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotConsumptionProductMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotConsumptionProductMutationViewResult FarmPlotConsumptionProductHttpController::update(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmPlotConsumptionProductUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmPlotConsumptionProductMutationViewResult::failure(errorView(result.error()));
    }
    return FarmPlotConsumptionProductMutationViewResult::success(
        fasc::server::views::FarmPlotConsumptionProductMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmPlotConsumptionProductMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmPlotConsumptionProductMutationViewResult FarmPlotConsumptionProductHttpController::erase(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmPlotConsumptionProductMutationViewResult::failure(errorView(result.error()));
  }
  return FarmPlotConsumptionProductMutationViewResult::success(
      fasc::server::views::FarmPlotConsumptionProductMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
