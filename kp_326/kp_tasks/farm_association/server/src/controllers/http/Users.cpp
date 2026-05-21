#include <controllers/http/Users.hpp>

#include <exception>

#include <marshalling/User.hpp>

#include <nlohmann/json.hpp>

#include <string>
#include <string_view>
#include <utility>

namespace fasc::server::controllers::http {

using fasc::server::controllers::dto::CreateUserCommand;
using fasc::server::controllers::dto::LoginUserCommand;
using fasc::server::controllers::dto::RegisterUserCommand;
using fasc::server::controllers::dto::UpdateUserRoleCommand;
using fasc::server::controllers::dto::UserDto;
using fasc::server::views::AuthView;
using fasc::server::views::ErrorView;
using fasc::server::views::ErrorViewCode;
using fasc::server::views::UserListView;
using fasc::server::views::UserView;

UserHttpController::UserHttpController(UserController& users) : users_(users) {}

UserViewResult UserHttpController::createUser(std::string_view body) {
  try {
    // Разбираем входной JSON.
    auto command = nlohmann::json::parse(body).get<CreateUserCommand>();

    // Маппим app result в view result.
    const auto result = users_.createUser(std::move(command));
    if (result.hasError()) {
      return UserViewResult::failure(errorView(result.error()));
    }

    const UserDto& user = result.success();
    return UserViewResult::success(UserView{user.login, user.role});
  } catch (const std::exception& exception) {
    return UserViewResult::failure(ErrorView{ErrorViewCode::BadRequest, exception.what()});
  }
}

UserListViewResult UserHttpController::listUsers() {
  const auto result = users_.listUsers();
  if (result.hasError()) {
    return UserListViewResult::failure(errorView(result.error()));
  }

  UserListView view;
  view.users.reserve(result.success().users.size());
  for (const UserDto& user : result.success().users) {
    view.users.push_back(UserView{user.login, user.role});
  }

  return UserListViewResult::success(std::move(view));
}

UserViewResult UserHttpController::updateUserRole(std::string login, std::string_view body) {
  try {
    auto command = nlohmann::json::parse(body).get<UpdateUserRoleCommand>();
    command.login = std::move(login);

    const auto result = users_.updateUserRole(std::move(command));
    if (result.hasError()) {
      return UserViewResult::failure(errorView(result.error()));
    }

    const UserDto& user = result.success();
    return UserViewResult::success(UserView{user.login, user.role});
  } catch (const std::exception& exception) {
    return UserViewResult::failure(ErrorView{ErrorViewCode::BadRequest, exception.what()});
  }
}

AuthViewResult UserHttpController::registerUser(std::string_view body) {
  try {
    // Разбираем входной JSON.
    auto command = nlohmann::json::parse(body).get<RegisterUserCommand>();

    // Маппим app result в view result.
    const auto result = users_.registerUser(std::move(command));
    if (result.hasError()) {
      return AuthViewResult::failure(errorView(result.error()));
    }

    const auto& auth = result.success();
    return AuthViewResult::success(
        AuthView{auth.token, auth.token_type, UserView{auth.user.login, auth.user.role}});
  } catch (const std::exception& exception) {
    return AuthViewResult::failure(ErrorView{ErrorViewCode::BadRequest, exception.what()});
  }
}

AuthViewResult UserHttpController::loginUser(std::string_view body) {
  try {
    auto command = nlohmann::json::parse(body).get<LoginUserCommand>();

    const auto result = users_.loginUser(std::move(command));
    if (result.hasError()) {
      return AuthViewResult::failure(errorView(result.error()));
    }

    const auto& auth = result.success();
    return AuthViewResult::success(
        AuthView{auth.token, auth.token_type, UserView{auth.user.login, auth.user.role}});
  } catch (const std::exception& exception) {
    return AuthViewResult::failure(ErrorView{ErrorViewCode::BadRequest, exception.what()});
  }
}

ErrorView UserHttpController::errorView(const fasc::server::controllers::app::UserError& error) {
  switch (error.code) {
  case fasc::server::controllers::app::UserErrorCode::InvalidInput:
    return ErrorView{ErrorViewCode::BadRequest, error.message};
  case fasc::server::controllers::app::UserErrorCode::Unauthorized:
    return ErrorView{ErrorViewCode::Unauthorized, error.message};
  case fasc::server::controllers::app::UserErrorCode::NotFound:
    return ErrorView{ErrorViewCode::NotFound, error.message};
  case fasc::server::controllers::app::UserErrorCode::Conflict:
    return ErrorView{ErrorViewCode::Conflict, error.message};
  case fasc::server::controllers::app::UserErrorCode::PersistenceFailure:
    return ErrorView{ErrorViewCode::InternalServerError, error.message};
  }

  return ErrorView{ErrorViewCode::InternalServerError, "Unknown user operation error"};
}

} // namespace fasc::server::controllers::http
