#include <controllers/http/Product.hpp>

#include <exception>

#include <marshalling/Product.hpp>

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

ProductHttpController::ProductHttpController(fasc::server::controllers::app::ProductController& controller)
    : controller_(controller) {}

ProductRowsViewResult ProductHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return ProductRowsViewResult::failure(errorView(result.error()));
  }
  return ProductRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

ProductRowViewResult ProductHttpController::load(
    const fasc::server::controllers::dto::ProductKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return ProductRowViewResult::failure(errorView(result.error()));
  }
  return ProductRowViewResult::success(fasc::server::views::toView(result.success().data));
}

ProductMutationViewResult ProductHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::ProductCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return ProductMutationViewResult::failure(errorView(result.error()));
    }
    return ProductMutationViewResult::success(
        fasc::server::views::ProductMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

ProductMutationViewResult ProductHttpController::update(
    const fasc::server::controllers::dto::ProductKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::ProductUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return ProductMutationViewResult::failure(errorView(result.error()));
    }
    return ProductMutationViewResult::success(
        fasc::server::views::ProductMutationView{result.success().affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

ProductMutationViewResult ProductHttpController::erase(
    const fasc::server::controllers::dto::ProductKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return ProductMutationViewResult::failure(errorView(result.error()));
  }
  return ProductMutationViewResult::success(
      fasc::server::views::ProductMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
