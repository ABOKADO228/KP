#pragma once

#include <utility>
#include <variant>

namespace fasc::extended::fpp {

///Результат операции, которая может завершиться успехом или предметной ошибкой.
///@tparam SuccessType тип успешного значения.
///@tparam ErrorType   тип ошибки.
template <typename SuccessType, typename ErrorType>
class Result {
public:
  ///Создает успешный результат.
  ///@param value значение успешного результата.
  ///@returns результат с @p value.
  static Result success(SuccessType value) {
    return Result{std::move(value)};
  }

  ///Создает ошибочный результат.
  ///@param error значение ошибки.
  ///@returns результат с @p error.
  static Result failure(ErrorType error) {
    return Result{std::move(error)};
  }

  ///Проверяет, содержит ли результат успешное значение.
  ///@returns @c true, если операция завершилась успешно.
  bool hasSuccess() const {
    return std::holds_alternative<SuccessType>(value_);
  }

  ///Проверяет, содержит ли результат ошибку.
  ///@returns @c true, если операция завершилась ошибкой.
  bool hasError() const {
    return std::holds_alternative<ErrorType>(value_);
  }

  ///Возвращает успешное значение.
  ///@returns ссылка на успешное значение.
  ///@throws @c std::bad_variant_access, если результат содержит ошибку.
  const SuccessType& success() const {
    return std::get<SuccessType>(value_);
  }

  ///Возвращает ошибку.
  ///@returns ссылка на ошибку.
  ///@throws @c std::bad_variant_access, если результат содержит успешное значение.
  const ErrorType& error() const {
    return std::get<ErrorType>(value_);
  }

private:
  explicit Result(SuccessType value) : value_(std::move(value)) {}
  explicit Result(ErrorType error) : value_(std::move(error)) {}

  std::variant<SuccessType, ErrorType> value_;
};
} // namespace fasc::extended::fpp
