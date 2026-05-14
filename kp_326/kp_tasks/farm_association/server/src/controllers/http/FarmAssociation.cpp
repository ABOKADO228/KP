#include <controllers/http/FarmAssociation.hpp>

#include <marshalling/FarmAssociation.hpp>

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

FarmAssociationHttpController::FarmAssociationHttpController(fasc::server::controllers::app::FarmAssociationController& controller)
    : controller_(controller) {}

FarmAssociationRowsViewResult FarmAssociationHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmAssociationRowsViewResult::failure(errorView(result.error()));
  }
  return FarmAssociationRowsViewResult::success(fasc::server::views::FarmAssociationRowsView{result.success().rows});
}

FarmAssociationRowViewResult FarmAssociationHttpController::load(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return FarmAssociationRowViewResult::failure(errorView(result.error()));
  }
  return FarmAssociationRowViewResult::success(fasc::server::views::FarmAssociationRowView{result.success().data});
}

FarmAssociationMutationViewResult FarmAssociationHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmAssociationCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return FarmAssociationMutationViewResult::failure(errorView(result.error()));
    }
    return FarmAssociationMutationViewResult::success(
        fasc::server::views::FarmAssociationMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmAssociationMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmAssociationMutationViewResult FarmAssociationHttpController::update(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::FarmAssociationUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return FarmAssociationMutationViewResult::failure(errorView(result.error()));
    }
    return FarmAssociationMutationViewResult::success(
        fasc::server::views::FarmAssociationMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return FarmAssociationMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

FarmAssociationMutationViewResult FarmAssociationHttpController::erase(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return FarmAssociationMutationViewResult::failure(errorView(result.error()));
  }
  return FarmAssociationMutationViewResult::success(
      fasc::server::views::FarmAssociationMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
