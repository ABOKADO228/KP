#include <controllers/http/PersonDocument.hpp>

#include <marshalling/PersonDocument.hpp>

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

PersonDocumentHttpController::PersonDocumentHttpController(fasc::server::controllers::app::PersonDocumentController& controller)
    : controller_(controller) {}

PersonDocumentRowsViewResult PersonDocumentHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return PersonDocumentRowsViewResult::failure(errorView(result.error()));
  }
  return PersonDocumentRowsViewResult::success(fasc::server::views::PersonDocumentRowsView{result.success().rows});
}

PersonDocumentRowViewResult PersonDocumentHttpController::load(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return PersonDocumentRowViewResult::failure(errorView(result.error()));
  }
  return PersonDocumentRowViewResult::success(fasc::server::views::PersonDocumentRowView{result.success().data});
}

PersonDocumentMutationViewResult PersonDocumentHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PersonDocumentCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return PersonDocumentMutationViewResult::failure(errorView(result.error()));
    }
    return PersonDocumentMutationViewResult::success(
        fasc::server::views::PersonDocumentMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PersonDocumentMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PersonDocumentMutationViewResult PersonDocumentHttpController::update(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PersonDocumentUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return PersonDocumentMutationViewResult::failure(errorView(result.error()));
    }
    return PersonDocumentMutationViewResult::success(
        fasc::server::views::PersonDocumentMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PersonDocumentMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PersonDocumentMutationViewResult PersonDocumentHttpController::erase(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return PersonDocumentMutationViewResult::failure(errorView(result.error()));
  }
  return PersonDocumentMutationViewResult::success(
      fasc::server::views::PersonDocumentMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
