#include <handling/Users.hpp>

#include <controllers/dto/User.hpp>
#include <marshalling/User.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

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

HttpResponse errorResponse(unsigned status, std::string message) {
  return HttpResponse{status, "application/json",
                      nlohmann::json(fasc::server::views::ErrorView{
                                         fasc::server::views::ErrorViewCode::Unauthorized,
                                         std::move(message)})
                          .dump()};
}

bool equalsIgnoreCase(std::string_view left, std::string_view right) {
  return left.size() == right.size() &&
         std::equal(left.begin(), left.end(), right.begin(), [](char a, char b) {
           return std::tolower(static_cast<unsigned char>(a)) ==
                  std::tolower(static_cast<unsigned char>(b));
         });
}

std::optional<std::string> headerValue(const HttpRequest& request, std::string_view name) {
  for (const auto& [header_name, header_value] : request.headers) {
    if (equalsIgnoreCase(header_name, name)) {
      return header_value;
    }
  }

  return std::nullopt;
}

std::optional<std::string> bearerTokenFrom(const HttpRequest& request) {
  const auto authorization = headerValue(request, "Authorization");
  if (!authorization.has_value()) {
    return std::nullopt;
  }

  constexpr std::string_view prefix = "Bearer ";
  if (authorization->size() <= prefix.size() ||
      !equalsIgnoreCase(std::string_view{authorization->data(), prefix.size()}, prefix)) {
    return std::nullopt;
  }

  return authorization->substr(prefix.size());
}

} // namespace

UserHandler::UserHandler(UserHttpController& users,
                         fasc::server::security::JwtService& jwt_service)
    : users_(users), jwt_service_(jwt_service) {}

HttpResponse UserHandler::createUser(const HttpRequest& request) {
  const auto token = bearerTokenFrom(request);
  if (!token.has_value()) {
    return errorResponse(401, "Authorization Bearer token is required");
  }

  const auto user = jwt_service_.verify(*token);
  if (!user.has_value()) {
    return errorResponse(401, "Authorization token is invalid or expired");
  }

  if (user->role != fasc::server::controllers::dto::kAdministratorUserRole) {
    return errorResponse(403, "Administrator role is required");
  }

  return responseFrom(users_.createUser(request.body), 201);
}

HttpResponse UserHandler::registerUser(const HttpRequest& request) {
  return responseFrom(users_.registerUser(request.body), 201);
}

HttpResponse UserHandler::loginUser(const HttpRequest& request) {
  return responseFrom(users_.loginUser(request.body), 200);
}

} // namespace fasc::server::handling
