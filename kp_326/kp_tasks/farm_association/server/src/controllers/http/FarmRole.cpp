#include <controllers/http/FarmRole.hpp>

#include <exception>

#include <marshalling/FarmRole.hpp>

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

FarmRoleHttpController::FarmRoleHttpController(fasc::server::controllers::app::FarmRoleController& controller)
    : controller_(controller) {}

FarmRoleRowsViewResult FarmRoleHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmRoleRowsViewResult::failure(errorView(result.error()));
  }
  return FarmRoleRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

FarmRoleRowViewResult FarmRoleHttpController::load(
    const fasc::server::controllers::dto::FarmRoleKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmRoleRowViewResult::failure(errorView(result.error()));
  }
  return FarmRoleRowViewResult::success(fasc::server::views::toView(result.success().data));
}

FarmRoleMutationViewResult FarmRoleHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmRoleCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmRoleMutationViewResult::failure(errorView(result.error()));
    }
    return FarmRoleMutationViewResult::success(
        fasc::server::views::FarmRoleMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmRoleMutationViewResult FarmRoleHttpController::update(
    const fasc::server::controllers::dto::FarmRoleKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmRoleUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmRoleMutationViewResult::failure(errorView(result.error()));
    }
    return FarmRoleMutationViewResult::success(
        fasc::server::views::FarmRoleMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmRoleMutationViewResult FarmRoleHttpController::erase(
    const fasc::server::controllers::dto::FarmRoleKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmRoleMutationViewResult::failure(errorView(result.error()));
  }
  return FarmRoleMutationViewResult::success(
      fasc::server::views::FarmRoleMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
