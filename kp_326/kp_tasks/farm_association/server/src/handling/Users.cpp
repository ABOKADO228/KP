#include <handling/Users.hpp>

#include <marshalling/User.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::handling {

namespace {

unsigned statusFrom(const fasc::server::views::ErrorView& error) {
  using fasc::server::views::ErrorViewCode;

  switch (error.code) {
  case ErrorViewCode::BadRequest:
    return 400;
  case ErrorViewCode::Unauthorized:
    return 401;
  case ErrorViewCode::NotFound:
    return 404;
  case ErrorViewCode::Conflict:
    return 409;
  case ErrorViewCode::InternalServerError:
    return 500;
  }

  return 500;
}

template <typename ResultType>
HttpResponse responseFrom(const ResultType& result, unsigned success_status) {
  // Ошибку сериализуем отдельно.
  if (result.hasError()) {
    return HttpResponse{statusFrom(result.error()), "application/json",
                        nlohmann::json(result.error()).dump()};
  }

  // Успешный view получает заданный статус.
  return HttpResponse{success_status, "application/json",
                      nlohmann::json(result.success()).dump()};
}

} // namespace

UserHandler::UserHandler(UserHttpController& users) : users_(users) {}

HttpResponse UserHandler::createUser(const HttpRequest& request) {
  return responseFrom(users_.createUser(request.body), 201);
}

HttpResponse UserHandler::registerUser(const HttpRequest& request) {
  return responseFrom(users_.registerUser(request.body), 201);
}

HttpResponse UserHandler::loginUser(const HttpRequest& request) {
  return responseFrom(users_.loginUser(request.body), 200);
}

} // namespace fasc::server::handling
