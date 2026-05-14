#include <controllers/http/IdentityDocumentType.hpp>

#include <marshalling/IdentityDocumentType.hpp>

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

IdentityDocumentTypeHttpController::IdentityDocumentTypeHttpController(fasc::server::controllers::app::IdentityDocumentTypeController& controller)
    : controller_(controller) {}

IdentityDocumentTypeRowsViewResult IdentityDocumentTypeHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return IdentityDocumentTypeRowsViewResult::failure(errorView(result.error()));
  }
  return IdentityDocumentTypeRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

IdentityDocumentTypeRowViewResult IdentityDocumentTypeHttpController::load(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return IdentityDocumentTypeRowViewResult::failure(errorView(result.error()));
  }
  return IdentityDocumentTypeRowViewResult::success(fasc::server::views::toView(result.success().data));
}

IdentityDocumentTypeMutationViewResult IdentityDocumentTypeHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::IdentityDocumentTypeCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return IdentityDocumentTypeMutationViewResult::failure(errorView(result.error()));
    }
    return IdentityDocumentTypeMutationViewResult::success(
        fasc::server::views::IdentityDocumentTypeMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return IdentityDocumentTypeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

IdentityDocumentTypeMutationViewResult IdentityDocumentTypeHttpController::update(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::IdentityDocumentTypeUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return IdentityDocumentTypeMutationViewResult::failure(errorView(result.error()));
    }
    return IdentityDocumentTypeMutationViewResult::success(
        fasc::server::views::IdentityDocumentTypeMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return IdentityDocumentTypeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

IdentityDocumentTypeMutationViewResult IdentityDocumentTypeHttpController::erase(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return IdentityDocumentTypeMutationViewResult::failure(errorView(result.error()));
  }
  return IdentityDocumentTypeMutationViewResult::success(
      fasc::server::views::IdentityDocumentTypeMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
