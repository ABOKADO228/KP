#include <controllers/http/AssociationMember.hpp>

#include <marshalling/AssociationMember.hpp>

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

AssociationMemberHttpController::AssociationMemberHttpController(fasc::server::controllers::app::AssociationMemberController& controller)
    : controller_(controller) {}

AssociationMemberRowsViewResult AssociationMemberHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return AssociationMemberRowsViewResult::failure(errorView(result.error()));
  }
  return AssociationMemberRowsViewResult::success(fasc::server::views::AssociationMemberRowsView{result.success().rows});
}

AssociationMemberRowViewResult AssociationMemberHttpController::load(
    const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return AssociationMemberRowViewResult::failure(errorView(result.error()));
  }
  return AssociationMemberRowViewResult::success(fasc::server::views::AssociationMemberRowView{result.success().data});
}

AssociationMemberMutationViewResult AssociationMemberHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationMemberCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return AssociationMemberMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationMemberMutationViewResult::success(
        fasc::server::views::AssociationMemberMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return AssociationMemberMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationMemberMutationViewResult AssociationMemberHttpController::update(
    const fasc::server::controllers::dto::AssociationMemberKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationMemberUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return AssociationMemberMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationMemberMutationViewResult::success(
        fasc::server::views::AssociationMemberMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return AssociationMemberMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationMemberMutationViewResult AssociationMemberHttpController::erase(
    const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return AssociationMemberMutationViewResult::failure(errorView(result.error()));
  }
  return AssociationMemberMutationViewResult::success(
      fasc::server::views::AssociationMemberMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
