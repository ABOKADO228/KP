#include <controllers/http/SupplierProductPrice.hpp>

#include <marshalling/SupplierProductPrice.hpp>

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

SupplierProductPriceHttpController::SupplierProductPriceHttpController(fasc::server::controllers::app::SupplierProductPriceController& controller)
    : controller_(controller) {}

SupplierProductPriceRowsViewResult SupplierProductPriceHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return SupplierProductPriceRowsViewResult::failure(errorView(result.error()));
  }
  return SupplierProductPriceRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

SupplierProductPriceRowViewResult SupplierProductPriceHttpController::load(
    const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return SupplierProductPriceRowViewResult::failure(errorView(result.error()));
  }
  return SupplierProductPriceRowViewResult::success(fasc::server::views::toView(result.success().data));
}

SupplierProductPriceMutationViewResult SupplierProductPriceHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::SupplierProductPriceCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return SupplierProductPriceMutationViewResult::failure(errorView(result.error()));
    }
    return SupplierProductPriceMutationViewResult::success(
        fasc::server::views::SupplierProductPriceMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return SupplierProductPriceMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

SupplierProductPriceMutationViewResult SupplierProductPriceHttpController::update(
    const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::SupplierProductPriceUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return SupplierProductPriceMutationViewResult::failure(errorView(result.error()));
    }
    return SupplierProductPriceMutationViewResult::success(
        fasc::server::views::SupplierProductPriceMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return SupplierProductPriceMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

SupplierProductPriceMutationViewResult SupplierProductPriceHttpController::erase(
    const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return SupplierProductPriceMutationViewResult::failure(errorView(result.error()));
  }
  return SupplierProductPriceMutationViewResult::success(
      fasc::server::views::SupplierProductPriceMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
