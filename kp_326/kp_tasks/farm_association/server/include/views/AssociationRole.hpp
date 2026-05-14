#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationRoleRowView {
  std::uint64_t id{};
  std::optional<fasc::server::domain::AssociationRoleCode> code{};
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
