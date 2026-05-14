#include <controllers/http/ProductType.hpp>

#include <marshalling/ProductType.hpp>

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

ProductTypeHttpController::ProductTypeHttpController(fasc::server::controllers::app::ProductTypeController& controller)
    : controller_(controller) {}

ProductTypeRowsViewResult ProductTypeHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return ProductTypeRowsViewResult::failure(errorView(result.error()));
  }
  return ProductTypeRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

ProductTypeRowViewResult ProductTypeHttpController::load(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return ProductTypeRowViewResult::failure(errorView(result.error()));
  }
  return ProductTypeRowViewResult::success(fasc::server::views::toView(result.success().data));
}

ProductTypeMutationViewResult ProductTypeHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::ProductTypeCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return ProductTypeMutationViewResult::failure(errorView(result.error()));
    }
    return ProductTypeMutationViewResult::success(
        fasc::server::views::ProductTypeMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return ProductTypeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

ProductTypeMutationViewResult ProductTypeHttpController::update(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::ProductTypeUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return ProductTypeMutationViewResult::failure(errorView(result.error()));
    }
    return ProductTypeMutationViewResult::success(
        fasc::server::views::ProductTypeMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return ProductTypeMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

ProductTypeMutationViewResult ProductTypeHttpController::erase(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return ProductTypeMutationViewResult::failure(errorView(result.error()));
  }
  return ProductTypeMutationViewResult::success(
      fasc::server::views::ProductTypeMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
