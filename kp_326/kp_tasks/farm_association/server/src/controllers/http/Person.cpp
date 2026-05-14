#include <controllers/http/Person.hpp>

#include <marshalling/Person.hpp>

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

PersonHttpController::PersonHttpController(fasc::server::controllers::app::PersonController& controller)
    : controller_(controller) {}

PersonRowsViewResult PersonHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return PersonRowsViewResult::failure(errorView(result.error()));
  }
  return PersonRowsViewResult::success(fasc::server::views::toView(result.success().rows));
}

PersonRowViewResult PersonHttpController::load(
    const fasc::server::controllers::dto::PersonKeyDto& key) const {
  const auto result = controller_.load(key);
  if (result.hasError()) {
    return PersonRowViewResult::failure(errorView(result.error()));
  }
  return PersonRowViewResult::success(fasc::server::views::toView(result.success().data));
}

PersonMutationViewResult PersonHttpController::create(std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PersonCreateDto>();
    const auto result = controller_.create(dto);
    if (result.hasError()) {
      return PersonMutationViewResult::failure(errorView(result.error()));
    }
    return PersonMutationViewResult::success(
        fasc::server::views::PersonMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PersonMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PersonMutationViewResult PersonHttpController::update(
    const fasc::server::controllers::dto::PersonKeyDto& key, std::string_view body) const {
  try {
    // Разбираем тело операции.
    const auto dto = nlohmann::json::parse(body).get<fasc::server::controllers::dto::PersonUpdateDto>();
    const auto result = controller_.update(key, dto);
    if (result.hasError()) {
      return PersonMutationViewResult::failure(errorView(result.error()));
    }
    return PersonMutationViewResult::success(
        fasc::server::views::PersonMutationView{result.success().affectedRows});
  } catch (const nlohmann::json::exception& exception) {
    return PersonMutationViewResult::failure(
        fasc::server::views::ErrorView{fasc::server::views::ErrorViewCode::BadRequest, exception.what()});
  }
}

PersonMutationViewResult PersonHttpController::erase(
    const fasc::server::controllers::dto::PersonKeyDto& key) const {
  const auto result = controller_.erase(key);
  if (result.hasError()) {
    return PersonMutationViewResult::failure(errorView(result.error()));
  }
  return PersonMutationViewResult::success(
      fasc::server::views::PersonMutationView{result.success().affectedRows});
}

} // namespace fasc::server::controllers::http
