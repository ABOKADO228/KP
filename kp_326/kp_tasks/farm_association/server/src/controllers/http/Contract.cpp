#include <controllers/http/Contract.hpp>

#include <marshalling/Contract.hpp>

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

ContractHttpController::ContractHttpController(fasc::server::controllers::app::ContractController& controller)
    : controller_(controller) {}

ContractRowsViewResult ContractHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return ContractRowsViewResult::failure(errorView(result.error()));
  }
  return ContractRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

ContractRowViewResult ContractHttpController::load(
    const fasc::server::controllers::dto::ContractKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return ContractRowViewResult::failure(errorView(result.error()));
  }
  return ContractRowViewResult::success(fasc::server::views::toView(result.success().data));
}

ContractMutationViewResult ContractHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::ContractCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return ContractMutationViewResult::failure(errorView(result.error()));
    }
    return ContractMutationViewResult::success(
        fasc::server::views::ContractMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return ContractMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

ContractMutationViewResult ContractHttpController::update(
    const fasc::server::controllers::dto::ContractKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::ContractUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return ContractMutationViewResult::failure(errorView(result.error()));
    }
    return ContractMutationViewResult::success(
        fasc::server::views::ContractMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return ContractMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

ContractMutationViewResult ContractHttpController::erase(
    const fasc::server::controllers::dto::ContractKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return ContractMutationViewResult::failure(errorView(result.error()));
  }
  return ContractMutationViewResult::success(
      fasc::server::views::ContractMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
