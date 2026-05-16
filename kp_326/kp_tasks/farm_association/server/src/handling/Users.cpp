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
                      nlohmann::json{{"error", std::move(message)}}.dump()};
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

bool canManageUsers(std::string_view role) {
  return role == fasc::server::controllers::dto::kAdministratorUserRole ||
         role == "association_director";
}

std::optional<HttpResponse> rejectUnlessCanManageUsers(
    const HttpRequest& request,
    fasc::server::security::JwtService& jwt_service) {
  const auto token = bearerTokenFrom(request);
  if (!token.has_value()) {
    return errorResponse(401, "Authorization Bearer token is required");
  }

  const auto user = jwt_service.verify(*token);
  if (!user.has_value()) {
    return errorResponse(401, "Authorization token is invalid or expired");
  }

  if (!canManageUsers(user->role)) {
    return errorResponse(403, "Administrator or association director role is required");
  }

  return std::nullopt;
}

} // namespace

UserHandler::UserHandler(UserHttpController& users,
                         fasc::server::security::JwtService& jwt_service)
    : users_(users), jwt_service_(jwt_service) {}

HttpResponse UserHandler::createUser(const HttpRequest& request) {
  if (auto rejection = rejectUnlessCanManageUsers(request, jwt_service_)) {
    return *rejection;
  }

  return responseFrom(users_.createUser(request.body), 201);
}

HttpResponse UserHandler::listUsers(const HttpRequest& request) {
  if (auto rejection = rejectUnlessCanManageUsers(request, jwt_service_)) {
    return *rejection;
  }

  return responseFrom(users_.listUsers(), 200);
}

HttpResponse UserHandler::updateUserRole(const HttpRequest& request) {
  if (auto rejection = rejectUnlessCanManageUsers(request, jwt_service_)) {
    return *rejection;
  }

  const auto login = request.query_params.find("login");
  if (login == request.query_params.end() || login->second.empty()) {
    return errorResponse(400, "User login query parameter is required");
  }

  return responseFrom(users_.updateUserRole(login->second, request.body), 200);
}

HttpResponse UserHandler::registerUser(const HttpRequest& request) {
  return responseFrom(users_.registerUser(request.body), 201);
}

HttpResponse UserHandler::loginUser(const HttpRequest& request) {
  return responseFrom(users_.loginUser(request.body), 200);
}

} // namespace fasc::server::handling
