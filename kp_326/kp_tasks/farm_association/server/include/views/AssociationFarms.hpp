#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationFarmsRowView {
  std::uint64_t farmId{};
  std::uint64_t associationId{};
  fasc::server::domain::Date joinDate{};
  std::optional<fasc::server::domain::AssociationFarmsStatus> status{};
  std::optional<std::string> notes{};
};

struct AssociationFarmsRowsView {
  std::vector<AssociationFarmsRowView> rows{};
};

struct AssociationFarmsMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
