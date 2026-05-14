#include <controllers/http/Unit.hpp>

#include <marshalling/Unit.hpp>

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

UnitHttpController::UnitHttpController(fasc::server::controllers::app::UnitController& controller)
    : controller_(controller) {}

UnitRowsViewResult UnitHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return UnitRowsViewResult::failure(errorView(result.error()));
  }
  return UnitRowsViewResult::success(fasc::server::views::UnitRowsView{result.success().rows});
}

UnitRowViewResult UnitHttpController::load(
    const fasc::server::controllers::dto::UnitKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return UnitRowViewResult::failure(errorView(result.error()));
  }
  return UnitRowViewResult::success(fasc::server::views::UnitRowView{result.success().data});
}

UnitMutationViewResult UnitHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::UnitCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return UnitMutationViewResult::failure(errorView(result.error()));
    }
    return UnitMutationViewResult::success(
        fasc::server::views::UnitMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return UnitMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

UnitMutationViewResult UnitHttpController::update(
    const fasc::server::controllers::dto::UnitKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::UnitUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return UnitMutationViewResult::failure(errorView(result.error()));
    }
    return UnitMutationViewResult::success(
        fasc::server::views::UnitMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return UnitMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

UnitMutationViewResult UnitHttpController::erase(
    const fasc::server::controllers::dto::UnitKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return UnitMutationViewResult::failure(errorView(result.error()));
  }
  return UnitMutationViewResult::success(
      fasc::server::views::UnitMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
