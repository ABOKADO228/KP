#pragma once

#include <string>
#include <utility>

struct ErrorType {
  std::string message;

  explicit ErrorType(std::string message_value) : message(std::move(message_value)) {}
  virtual ~ErrorType() = default;
};

struct ValidationError : ErrorType {
  using ErrorType::ErrorType;
};

struct NotFoundError : ErrorType {
  using ErrorType::ErrorType;
};

struct ConflictError : ErrorType {
  using ErrorType::ErrorType;
};

struct UnauthorizedError : ErrorType {
  using ErrorType::ErrorType;
};

struct DatabaseError : ErrorType {
  using ErrorType::ErrorType;
};

struct HttpError : ErrorType {
  unsigned status{500};

  HttpError(unsigned status_code, std::string message_value)
      : ErrorType(std::move(message_value)), status(status_code) {}

  explicit HttpError(const ValidationError& error) : HttpError(400, error.message) {}
  explicit HttpError(const NotFoundError& error) : HttpError(404, error.message) {}
  explicit HttpError(const ConflictError& error) : HttpError(409, error.message) {}
  explicit HttpError(const UnauthorizedError& error) : HttpError(401, error.message) {}
  explicit HttpError(const DatabaseError& error) : HttpError(500, error.message) {}
};
