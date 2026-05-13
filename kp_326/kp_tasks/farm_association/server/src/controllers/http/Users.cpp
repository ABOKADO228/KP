#include <controllers/http/Users.hpp>

#include <marshalling/User.hpp>
#include <server/core/HttpResult.hpp>

#include <nlohmann/json.hpp>

#include <stdexcept>
#include <string>

namespace {

HttpError user_error_from(const std::invalid_argument& exception) {
  const std::string message = exception.what();
  if (message == "User name is already registered") {
    return HttpError{ConflictError{message}};
  }
  if (message == "Invalid user name or password") {
    return HttpError{UnauthorizedError{message}};
  }
  return HttpError{ValidationError{message}};
}

} // namespace

UserHttpController::UserHttpController(UserController& users) : users_(users) {}

HttpResponse UserHttpController::create_user(const HttpRequest& request) {
  HttpResult result = HttpResult::fail(HttpError{ValidationError{"Unhandled request"}});
  try {
    auto command = nlohmann::json::parse(request.body).get<CreateUserCommand>();

    const UserDto user = users_.create_user(std::move(command));
    const UserView view{user.id, user.name};
    const nlohmann::json body = view;

    result = HttpResult::ok(HttpResponse{201, "application/json", body.dump()});
  } catch (const std::invalid_argument& exception) {
    result = HttpResult::fail(user_error_from(exception));
  } catch (const nlohmann::json::exception& exception) {
    result = HttpResult::fail(HttpError{ValidationError{exception.what()}});
  }

  return unwrapHttpResult(result);
}

HttpResponse UserHttpController::register_user(const HttpRequest& request) {
  HttpResult result = HttpResult::fail(HttpError{ValidationError{"Unhandled request"}});
  try {
    auto command = nlohmann::json::parse(request.body).get<RegisterUserCommand>();
    result = HttpResult::ok(auth_response(users_.register_user(std::move(command)), 201));
  } catch (const std::invalid_argument& exception) {
    result = HttpResult::fail(user_error_from(exception));
  } catch (const nlohmann::json::exception& exception) {
    result = HttpResult::fail(HttpError{ValidationError{exception.what()}});
  }

  return unwrapHttpResult(result);
}

HttpResponse UserHttpController::login_user(const HttpRequest& request) {
  HttpResult result = HttpResult::fail(HttpError{ValidationError{"Unhandled request"}});
  try {
    auto command = nlohmann::json::parse(request.body).get<LoginUserCommand>();
    result = HttpResult::ok(auth_response(users_.login_user(std::move(command)), 200));
  } catch (const std::invalid_argument& exception) {
    result = HttpResult::fail(user_error_from(exception));
  } catch (const nlohmann::json::exception& exception) {
    result = HttpResult::fail(HttpError{ValidationError{exception.what()}});
  }

  return unwrapHttpResult(result);
}

HttpResponse UserHttpController::auth_response(const AuthResultDto& result, unsigned status) const {
  const nlohmann::json body = {
      {"token", result.token},
      {"token_type", result.token_type},
      {"user", UserView{result.user.id, result.user.name}},
  };

  return HttpResponse{status, "application/json", body.dump()};
}
