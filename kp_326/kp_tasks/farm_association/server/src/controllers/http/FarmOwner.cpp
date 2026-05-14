#include <controllers/http/FarmOwner.hpp>

#include <exception>

#include <marshalling/FarmOwner.hpp>

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

FarmOwnerHttpController::FarmOwnerHttpController(fasc::server::controllers::app::FarmOwnerController& controller)
    : controller_(controller) {}

FarmOwnerRowsViewResult FarmOwnerHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmOwnerRowsViewResult::failure(errorView(result.error()));
  }
  return FarmOwnerRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmOwnerRowViewResult FarmOwnerHttpController::load(
    const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmOwnerRowViewResult::failure(errorView(result.error()));
  }
  return FarmOwnerRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmOwnerMutationViewResult FarmOwnerHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmOwnerCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmOwnerMutationViewResult::failure(errorView(result.error()));
    }
    return FarmOwnerMutationViewResult::success(
        fasc::server::views::FarmOwnerMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmOwnerMutationViewResult FarmOwnerHttpController::update(
    const fasc::server::controllers::dto::FarmOwnerKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmOwnerUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmOwnerMutationViewResult::failure(errorView(result.error()));
    }
    return FarmOwnerMutationViewResult::success(
        fasc::server::views::FarmOwnerMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmOwnerMutationViewResult FarmOwnerHttpController::erase(
    const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmOwnerMutationViewResult::failure(errorView(result.error()));
  }
  return FarmOwnerMutationViewResult::success(
      fasc::server::views::FarmOwnerMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
