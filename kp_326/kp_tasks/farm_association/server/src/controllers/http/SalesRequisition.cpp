#include <controllers/http/SalesRequisition.hpp>

#include <marshalling/SalesRequisition.hpp>

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

SalesRequisitionHttpController::SalesRequisitionHttpController(fasc::server::controllers::app::SalesRequisitionController& controller)
    : controller_(controller) {}

SalesRequisitionRowsViewResult SalesRequisitionHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return SalesRequisitionRowsViewResult::failure(errorView(result.error()));
  }
  return SalesRequisitionRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

SalesRequisitionRowViewResult SalesRequisitionHttpController::load(
    const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return SalesRequisitionRowViewResult::failure(errorView(result.error()));
  }
  return SalesRequisitionRowViewResult::success(fasc::server::views::toView(result.success().data));
}

SalesRequisitionMutationViewResult SalesRequisitionHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::SalesRequisitionCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return SalesRequisitionMutationViewResult::failure(errorView(result.error()));
    }
    return SalesRequisitionMutationViewResult::success(
        fasc::server::views::SalesRequisitionMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return SalesRequisitionMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

SalesRequisitionMutationViewResult SalesRequisitionHttpController::update(
    const fasc::server::controllers::dto::SalesRequisitionKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::SalesRequisitionUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return SalesRequisitionMutationViewResult::failure(errorView(result.error()));
    }
    return SalesRequisitionMutationViewResult::success(
        fasc::server::views::SalesRequisitionMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return SalesRequisitionMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

SalesRequisitionMutationViewResult SalesRequisitionHttpController::erase(
    const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return SalesRequisitionMutationViewResult::failure(errorView(result.error()));
  }
  return SalesRequisitionMutationViewResult::success(
      fasc::server::views::SalesRequisitionMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
