#include <server/core/RequestDispatcher.hpp>

#include <cctype>
#include <string>
#include <unordered_map>

namespace fasc::server::core {

namespace {

std::string urlDecode(std::string_view value) {
  std::string decoded;
  decoded.reserve(value.size());

  for (std::size_t i = 0; i < value.size(); ++i) {
    if (value[i] == '+') {
      decoded.push_back(' ');
    } else if (value[i] == '%' && i + 2 < value.size() &&
               std::isxdigit(static_cast<unsigned char>(value[i + 1])) &&
               std::isxdigit(static_cast<unsigned char>(value[i + 2]))) {
      const std::string hex{value.substr(i + 1, 2)};
      decoded.push_back(static_cast<char>(std::stoi(hex, nullptr, 16)));
      i += 2;
    } else {
      decoded.push_back(value[i]);
    }
  }

  return decoded;
}

std::unordered_map<std::string, std::string> queryParamsFrom(std::string_view target) {
  std::unordered_map<std::string, std::string> params;
  const auto query_position = target.find('?');
  if (query_position == std::string_view::npos) {
    return params;
  }

  std::string_view query = target.substr(query_position + 1);
  while (!query.empty()) {
    const auto amp = query.find('&');
    const std::string_view pair = query.substr(0, amp);
    const auto eq = pair.find('=');
    if (eq != std::string_view::npos) {
      params.emplace(urlDecode(pair.substr(0, eq)), urlDecode(pair.substr(eq + 1)));
    } else if (!pair.empty()) {
      params.emplace(urlDecode(pair), "");
    }

    if (amp == std::string_view::npos) {
      break;
    }
    query.remove_prefix(amp + 1);
  }

  return params;
}

std::string routeTargetFrom(std::string target) {
  const auto query_position = target.find('?');
  if (query_position != std::string::npos) {
    target.resize(query_position);
  }

  return target.empty() ? "/" : target;
}

void applyCorsHeaders(BeastResponse& response) {
  response.set("Access-Control-Allow-Origin", "*");
  response.set("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  response.set("Access-Control-Allow-Headers", "Content-Type, Authorization");
}

} // namespace

RequestDispatcher::RequestDispatcher(const AppRouter& router) : router_(router) {}

BeastResponse RequestDispatcher::dispatch(const BeastRequest& request) const {
  if (request.method() == http::verb::options) {
    BeastResponse response{http::status::no_content, request.version()};
    response.set(http::field::server, "farm-association-server");
    response.set("X-Content-Type-Options", "nosniff");
    applyCorsHeaders(response);
    response.keep_alive(request.keep_alive());
    response.prepare_payload();
    return response;
  }

  HttpRequest app_request;
  app_request.method = std::string(request.method_string());
  const std::string raw_target{request.target()};
  app_request.target = routeTargetFrom(raw_target);
  app_request.query_params = queryParamsFrom(raw_target);
  app_request.body = request.body();

  for (const auto& field : request) {
    app_request.headers.emplace(std::string(field.name_string()), std::string(field.value()));
  }

  const HttpResponse app_response = router_.route(app_request);

  BeastResponse response{static_cast<http::status>(app_response.status), request.version()};

  response.set(http::field::server, "farm-association-server");
  response.set(http::field::content_type, app_response.content_type);
  response.set("X-Content-Type-Options", "nosniff");
  applyCorsHeaders(response);
  response.keep_alive(request.keep_alive());
  response.body() = app_response.body;
  response.prepare_payload();

  return response;
}

} // namespace fasc::server::core
