#include <controllers/http/PurchaseOrder.hpp>

#include <marshalling/PurchaseOrder.hpp>

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

PurchaseOrderHttpController::PurchaseOrderHttpController(fasc::server::controllers::app::PurchaseOrderController& controller)
    : controller_(controller) {}

PurchaseOrderRowsViewResult PurchaseOrderHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return PurchaseOrderRowsViewResult::failure(errorView(result.error()));
  }
  return PurchaseOrderRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

PurchaseOrderRowViewResult PurchaseOrderHttpController::load(
    const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return PurchaseOrderRowViewResult::failure(errorView(result.error()));
  }
  return PurchaseOrderRowViewResult::success(fasc::server::views::toView(result.success().data));
}

PurchaseOrderMutationViewResult PurchaseOrderHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PurchaseOrderCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return PurchaseOrderMutationViewResult::failure(errorView(result.error()));
    }
    return PurchaseOrderMutationViewResult::success(
        fasc::server::views::PurchaseOrderMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PurchaseOrderMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PurchaseOrderMutationViewResult PurchaseOrderHttpController::update(
    const fasc::server::controllers::dto::PurchaseOrderKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PurchaseOrderUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return PurchaseOrderMutationViewResult::failure(errorView(result.error()));
    }
    return PurchaseOrderMutationViewResult::success(
        fasc::server::views::PurchaseOrderMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PurchaseOrderMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PurchaseOrderMutationViewResult PurchaseOrderHttpController::erase(
    const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return PurchaseOrderMutationViewResult::failure(errorView(result.error()));
  }
  return PurchaseOrderMutationViewResult::success(
      fasc::server::views::PurchaseOrderMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
