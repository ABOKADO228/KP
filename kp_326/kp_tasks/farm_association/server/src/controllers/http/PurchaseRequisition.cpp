#include <controllers/http/PurchaseRequisition.hpp>

#include <marshalling/PurchaseRequisition.hpp>

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

PurchaseRequisitionHttpController::PurchaseRequisitionHttpController(fasc::server::controllers::app::PurchaseRequisitionController& controller)
    : controller_(controller) {}

PurchaseRequisitionRowsViewResult PurchaseRequisitionHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return PurchaseRequisitionRowsViewResult::failure(errorView(result.error()));
  }
  return PurchaseRequisitionRowsViewResult::success(fasc::server::views::PurchaseRequisitionRowsView{result.success().rows});
}

PurchaseRequisitionRowViewResult PurchaseRequisitionHttpController::load(
    const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return PurchaseRequisitionRowViewResult::failure(errorView(result.error()));
  }
  return PurchaseRequisitionRowViewResult::success(fasc::server::views::PurchaseRequisitionRowView{result.success().data});
}

PurchaseRequisitionMutationViewResult PurchaseRequisitionHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PurchaseRequisitionCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return PurchaseRequisitionMutationViewResult::failure(errorView(result.error()));
    }
    return PurchaseRequisitionMutationViewResult::success(
        fasc::server::views::PurchaseRequisitionMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PurchaseRequisitionMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PurchaseRequisitionMutationViewResult PurchaseRequisitionHttpController::update(
    const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PurchaseRequisitionUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return PurchaseRequisitionMutationViewResult::failure(errorView(result.error()));
    }
    return PurchaseRequisitionMutationViewResult::success(
        fasc::server::views::PurchaseRequisitionMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PurchaseRequisitionMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PurchaseRequisitionMutationViewResult PurchaseRequisitionHttpController::erase(
    const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return PurchaseRequisitionMutationViewResult::failure(errorView(result.error()));
  }
  return PurchaseRequisitionMutationViewResult::success(
      fasc::server::views::PurchaseRequisitionMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
