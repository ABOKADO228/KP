#include <controllers/http/AssociationRole.hpp>

#include <marshalling/AssociationRole.hpp>

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

AssociationRoleHttpController::AssociationRoleHttpController(fasc::server::controllers::app::AssociationRoleController& controller)
    : controller_(controller) {}

AssociationRoleRowsViewResult AssociationRoleHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return AssociationRoleRowsViewResult::failure(errorView(result.error()));
  }
  return AssociationRoleRowsViewResult::success(fasc::server::views::AssociationRoleRowsView{result.success().rows});
}

AssociationRoleRowViewResult AssociationRoleHttpController::load(
    const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return AssociationRoleRowViewResult::failure(errorView(result.error()));
  }
  return AssociationRoleRowViewResult::success(fasc::server::views::AssociationRoleRowView{result.success().data});
}

AssociationRoleMutationViewResult AssociationRoleHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationRoleCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return AssociationRoleMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationRoleMutationViewResult::success(
        fasc::server::views::AssociationRoleMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return AssociationRoleMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationRoleMutationViewResult AssociationRoleHttpController::update(
    const fasc::server::controllers::dto::AssociationRoleKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationRoleUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return AssociationRoleMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationRoleMutationViewResult::success(
        fasc::server::views::AssociationRoleMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return AssociationRoleMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationRoleMutationViewResult AssociationRoleHttpController::erase(
    const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return AssociationRoleMutationViewResult::failure(errorView(result.error()));
  }
  return AssociationRoleMutationViewResult::success(
      fasc::server::views::AssociationRoleMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
