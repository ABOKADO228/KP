#pragma once

#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

template <typename Value, typename Error>
class Result {
public:
  using value_type = Value;
  using error_type = Error;

  static Result ok(Value value) {
    return Result(std::move(value));
  }

  static Result fail(Error error) {
    return Result(std::move(error));
  }

  bool has_value() const noexcept {
    return std::holds_alternative<Value>(storage_);
  }

  bool has_error() const noexcept {
    return !has_value();
  }

  explicit operator bool() const noexcept {
    return has_value();
  }

  Value& value() & {
    if (!has_value()) {
      throw std::logic_error{"Result does not contain a value"};
    }
    return std::get<Value>(storage_);
  }

  const Value& value() const& {
    if (!has_value()) {
      throw std::logic_error{"Result does not contain a value"};
    }
    return std::get<Value>(storage_);
  }

  Value&& value() && {
    if (!has_value()) {
      throw std::logic_error{"Result does not contain a value"};
    }
    return std::move(std::get<Value>(storage_));
  }

  Error& error() & {
    if (has_value()) {
      throw std::logic_error{"Result does not contain an error"};
    }
    return std::get<Error>(storage_);
  }

  const Error& error() const& {
    if (has_value()) {
      throw std::logic_error{"Result does not contain an error"};
    }
    return std::get<Error>(storage_);
  }

  Error&& error() && {
    if (has_value()) {
      throw std::logic_error{"Result does not contain an error"};
    }
    return std::move(std::get<Error>(storage_));
  }

  template <typename F>
  auto map(F&& mapper) const& -> Result<std::invoke_result_t<F, const Value&>, Error> {
    using Mapped = std::invoke_result_t<F, const Value&>;
    if (has_error()) {
      return Result<Mapped, Error>::fail(error());
    }
    return Result<Mapped, Error>::ok(std::forward<F>(mapper)(value()));
  }

  template <typename F>
  auto map(F&& mapper) && -> Result<std::invoke_result_t<F, Value&&>, Error> {
    using Mapped = std::invoke_result_t<F, Value&&>;
    if (has_error()) {
      return Result<Mapped, Error>::fail(std::move(*this).error());
    }
    return Result<Mapped, Error>::ok(std::forward<F>(mapper)(std::move(*this).value()));
  }

  template <typename F>
  auto and_then(F&& next) const& -> std::invoke_result_t<F, const Value&> {
    using NextResult = std::invoke_result_t<F, const Value&>;
    if (has_error()) {
      return NextResult::fail(error());
    }
    return std::forward<F>(next)(value());
  }

  template <typename F>
  auto and_then(F&& next) && -> std::invoke_result_t<F, Value&&> {
    using NextResult = std::invoke_result_t<F, Value&&>;
    if (has_error()) {
      return NextResult::fail(std::move(*this).error());
    }
    return std::forward<F>(next)(std::move(*this).value());
  }

  template <typename F>
  Result or_else(F&& recover) const& {
    if (has_value()) {
      return *this;
    }
    return std::forward<F>(recover)(error());
  }

  template <typename F>
  Result or_else(F&& recover) && {
    if (has_value()) {
      return std::move(*this);
    }
    return std::forward<F>(recover)(std::move(*this).error());
  }

  template <typename F>
  Value unwrap_or_else(F&& fallback) const& {
    if (has_value()) {
      return value();
    }
    return std::forward<F>(fallback)(error());
  }

  Value value_or(Value fallback) const& {
    if (has_value()) {
      return value();
    }
    return fallback;
  }

private:
  explicit Result(Value value) : storage_(std::move(value)) {}
  explicit Result(Error error) : storage_(std::move(error)) {}

  std::variant<Value, Error> storage_;
};
