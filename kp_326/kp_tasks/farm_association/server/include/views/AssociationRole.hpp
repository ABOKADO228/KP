#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationRoleRowView {
  int id{};
  std::optional<std::string> code{};
  std::optional<std::string> name{};
  std::optional<std::string> description{};
};

struct AssociationRoleRowsView {
  std::vector<AssociationRoleRowView> rows{};
};

struct AssociationRoleMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
