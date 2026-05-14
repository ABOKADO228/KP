#include <controllers/http/PurchaseOrderItem.hpp>

#include <marshalling/PurchaseOrderItem.hpp>

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

PurchaseOrderItemHttpController::PurchaseOrderItemHttpController(fasc::server::controllers::app::PurchaseOrderItemController& controller)
    : controller_(controller) {}

PurchaseOrderItemRowsViewResult PurchaseOrderItemHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return PurchaseOrderItemRowsViewResult::failure(errorView(result.error()));
  }
  return PurchaseOrderItemRowsViewResult::success(fasc::server::views::PurchaseOrderItemRowsView{result.success().rows});
}

PurchaseOrderItemRowViewResult PurchaseOrderItemHttpController::load(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return PurchaseOrderItemRowViewResult::failure(errorView(result.error()));
  }
  return PurchaseOrderItemRowViewResult::success(fasc::server::views::PurchaseOrderItemRowView{result.success().data});
}

PurchaseOrderItemMutationViewResult PurchaseOrderItemHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PurchaseOrderItemCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return PurchaseOrderItemMutationViewResult::failure(errorView(result.error()));
    }
    return PurchaseOrderItemMutationViewResult::success(
        fasc::server::views::PurchaseOrderItemMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PurchaseOrderItemMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PurchaseOrderItemMutationViewResult PurchaseOrderItemHttpController::update(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PurchaseOrderItemUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return PurchaseOrderItemMutationViewResult::failure(errorView(result.error()));
    }
    return PurchaseOrderItemMutationViewResult::success(
        fasc::server::views::PurchaseOrderItemMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PurchaseOrderItemMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PurchaseOrderItemMutationViewResult PurchaseOrderItemHttpController::erase(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return PurchaseOrderItemMutationViewResult::failure(errorView(result.error()));
  }
  return PurchaseOrderItemMutationViewResult::success(
      fasc::server::views::PurchaseOrderItemMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
