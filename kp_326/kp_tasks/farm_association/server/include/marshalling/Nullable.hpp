#pragma once

#include <odb/nullable.hxx>

#include <optional>

namespace fasc::server::views::detail {

template <typename T>
inline std::optional<T> toOptional(const odb::nullable<T>& value) {
  if (value.null()) {
    return std::nullopt;
  }
  return value.get();
}

} // namespace fasc::server::views::detail
