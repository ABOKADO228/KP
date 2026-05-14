#include <controllers/http/Farm.hpp>

#include <exception>

#include <marshalling/Farm.hpp>

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

FarmHttpController::FarmHttpController(fasc::server::controllers::app::FarmController& controller)
    : controller_(controller) {}

FarmRowsViewResult FarmHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmRowsViewResult::failure(errorView(result.error()));
  }
  return FarmRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmRowViewResult FarmHttpController::load(
    const fasc::server::controllers::dto::FarmKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmRowViewResult::failure(errorView(result.error()));
  }
  return FarmRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmMutationViewResult FarmHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmMutationViewResult::failure(errorView(result.error()));
    }
    return FarmMutationViewResult::success(
        fasc::server::views::FarmMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmMutationViewResult FarmHttpController::update(
    const fasc::server::controllers::dto::FarmKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmMutationViewResult::failure(errorView(result.error()));
    }
    return FarmMutationViewResult::success(
        fasc::server::views::FarmMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmMutationViewResult FarmHttpController::erase(
    const fasc::server::controllers::dto::FarmKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmMutationViewResult::failure(errorView(result.error()));
  }
  return FarmMutationViewResult::success(
      fasc::server::views::FarmMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
