#pragma once

#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

namespace fasc::server::handling {

inline std::uint64_t parseUint64KeyField(std::string_view value, std::string_view fieldName) {
  if (value.empty()) {
    throw std::invalid_argument{"Invalid key field: " + std::string{fieldName}};
  }
  std::uint64_t result{};
  const auto* begin = value.data();
  const auto* end = begin + value.size();
  const auto [position, error] = std::from_chars(begin, end, result);
  if (error != std::errc{} || position != end) {
    throw std::invalid_argument{"Invalid key field: " + std::string{fieldName}};
  }
  return result;
}

} // namespace fasc::server::handling
