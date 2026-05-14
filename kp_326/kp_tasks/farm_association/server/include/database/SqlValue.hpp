#pragma once

#include <database/Database.hpp>
#include <domain/Types.hpp>

#include <optional>
#include <stdexcept>
#include <string>

namespace fasc::server::database {

template <typename T>
inline SqlParameter makeSqlParameter(const T& value) {
  return SqlParameter{fasc::server::domain::toSqlText(value), false};
}

template <typename T>
inline T requireColumn(const SqlRow& row, const std::string& column) {
  const auto it = row.find(column);
  if (it == row.end() || !it->second.has_value()) {
    throw std::runtime_error{"Column is null: " + column};
  }
  return fasc::server::domain::parseSqlText<T>(*it->second);
}

template <typename T>
inline std::optional<T> optionalColumn(const SqlRow& row, const std::string& column) {
  const auto it = row.find(column);
  if (it == row.end()) {
    throw std::runtime_error{"Column is missing: " + column};
  }
  if (!it->second.has_value()) {
    return std::nullopt;
  }
  return fasc::server::domain::parseSqlText<T>(*it->second);
}

} // namespace fasc::server::database
