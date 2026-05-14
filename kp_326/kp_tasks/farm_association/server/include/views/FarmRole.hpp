#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmRoleRowView {
  std::uint64_t id{};
  std::optional<std::string> name{};
  std::optional<std::string> description{};
};

struct FarmRoleRowsView {
  std::vector<FarmRoleRowView> rows{};
};

struct FarmRoleMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
