#include <server/core/HttpResult.hpp>

#include <nlohmann/json.hpp>

HttpResponse errorResponse(const HttpError& error) {
  const nlohmann::json body = {
      {"error", error.message},
  };
  return HttpResponse{error.status, "application/json", body.dump()};
}

HttpResponse unwrapHttpResult(const HttpResult& result) {
  if (result) {
    return result.value();
  }

  return errorResponse(result.error());
}
