#include <controllers/http/Supplier.hpp>

#include <marshalling/Supplier.hpp>

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

SupplierHttpController::SupplierHttpController(fasc::server::controllers::app::SupplierController& controller)
    : controller_(controller) {}

SupplierRowsViewResult SupplierHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return SupplierRowsViewResult::failure(errorView(result.error()));
  }
  return SupplierRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

SupplierRowViewResult SupplierHttpController::load(
    const fasc::server::controllers::dto::SupplierKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return SupplierRowViewResult::failure(errorView(result.error()));
  }
  return SupplierRowViewResult::success(fasc::server::views::toView(result.success().data));
}

SupplierMutationViewResult SupplierHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::SupplierCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return SupplierMutationViewResult::failure(errorView(result.error()));
    }
    return SupplierMutationViewResult::success(
        fasc::server::views::SupplierMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return SupplierMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

SupplierMutationViewResult SupplierHttpController::update(
    const fasc::server::controllers::dto::SupplierKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::SupplierUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return SupplierMutationViewResult::failure(errorView(result.error()));
    }
    return SupplierMutationViewResult::success(
        fasc::server::views::SupplierMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return SupplierMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

SupplierMutationViewResult SupplierHttpController::erase(
    const fasc::server::controllers::dto::SupplierKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return SupplierMutationViewResult::failure(errorView(result.error()));
  }
  return SupplierMutationViewResult::success(
      fasc::server::views::SupplierMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
