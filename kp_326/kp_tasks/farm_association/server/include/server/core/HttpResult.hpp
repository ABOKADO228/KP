#pragma once

#include <core/Error.hpp>
#include <core/Result.hpp>
#include <server/core/HttpTypes.hpp>

using HttpResult = Result<HttpResponse, HttpError>;

HttpResponse errorResponse(const HttpError& error);
HttpResponse unwrapHttpResult(const HttpResult& result);
