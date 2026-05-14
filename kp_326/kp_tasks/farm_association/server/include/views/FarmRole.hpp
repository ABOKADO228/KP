#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmRoleRowView {
  int id{};
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
