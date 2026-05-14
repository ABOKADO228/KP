#include <controllers/http/FarmOwnership.hpp>

#include <exception>

#include <marshalling/FarmOwnership.hpp>

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

FarmOwnershipHttpController::FarmOwnershipHttpController(fasc::server::controllers::app::FarmOwnershipController& controller)
    : controller_(controller) {}

FarmOwnershipRowsViewResult FarmOwnershipHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmOwnershipRowsViewResult::failure(errorView(result.error()));
  }
  return FarmOwnershipRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmOwnershipRowViewResult FarmOwnershipHttpController::load(
    const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmOwnershipRowViewResult::failure(errorView(result.error()));
  }
  return FarmOwnershipRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmOwnershipMutationViewResult FarmOwnershipHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmOwnershipCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmOwnershipMutationViewResult::failure(errorView(result.error()));
    }
    return FarmOwnershipMutationViewResult::success(
        fasc::server::views::FarmOwnershipMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmOwnershipMutationViewResult FarmOwnershipHttpController::update(
    const fasc::server::controllers::dto::FarmOwnershipKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmOwnershipUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmOwnershipMutationViewResult::failure(errorView(result.error()));
    }
    return FarmOwnershipMutationViewResult::success(
        fasc::server::views::FarmOwnershipMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmOwnershipMutationViewResult FarmOwnershipHttpController::erase(
    const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmOwnershipMutationViewResult::failure(errorView(result.error()));
  }
  return FarmOwnershipMutationViewResult::success(
      fasc::server::views::FarmOwnershipMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
