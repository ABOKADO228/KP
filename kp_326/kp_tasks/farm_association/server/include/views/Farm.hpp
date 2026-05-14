#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmRowView {
  std::uint64_t id{};
  std::optional<std::string> name{};
  std::optional<std::string> legalName{};
  std::optional<std::string> legalAddress{};
  std::optional<std::string> actualAddress{};
  std::optional<std::string> inn{};
  std::optional<std::string> ogrn{};
  std::optional<fasc::server::domain::FarmStatus> status{};
  std::optional<fasc::server::domain::FarmType> farmType{};
};

struct FarmRowsView {
  std::vector<FarmRowView> rows{};
};

struct FarmMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
