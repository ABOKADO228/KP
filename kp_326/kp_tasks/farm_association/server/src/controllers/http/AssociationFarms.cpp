#include <controllers/http/AssociationFarms.hpp>

#include <marshalling/AssociationFarms.hpp>

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

AssociationFarmsHttpController::AssociationFarmsHttpController(fasc::server::controllers::app::AssociationFarmsController& controller)
    : controller_(controller) {}

AssociationFarmsRowsViewResult AssociationFarmsHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return AssociationFarmsRowsViewResult::failure(errorView(result.error()));
  }
  return AssociationFarmsRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

AssociationFarmsRowViewResult AssociationFarmsHttpController::load(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return AssociationFarmsRowViewResult::failure(errorView(result.error()));
  }
  return AssociationFarmsRowViewResult::success(fasc::server::views::toView(result.success().data));
}

AssociationFarmsMutationViewResult AssociationFarmsHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationFarmsCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return AssociationFarmsMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationFarmsMutationViewResult::success(
        fasc::server::views::AssociationFarmsMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return AssociationFarmsMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationFarmsMutationViewResult AssociationFarmsHttpController::update(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::AssociationFarmsUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return AssociationFarmsMutationViewResult::failure(errorView(result.error()));
    }
    return AssociationFarmsMutationViewResult::success(
        fasc::server::views::AssociationFarmsMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return AssociationFarmsMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

AssociationFarmsMutationViewResult AssociationFarmsHttpController::erase(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return AssociationFarmsMutationViewResult::failure(errorView(result.error()));
  }
  return AssociationFarmsMutationViewResult::success(
      fasc::server::views::AssociationFarmsMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
