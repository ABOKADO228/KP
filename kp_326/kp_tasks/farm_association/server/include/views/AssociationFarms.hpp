#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationFarmsRowView {
  int farmId{};
  int associationId{};
  std::string joinDate{};
  std::optional<std::string> status{};
  std::optional<std::string> notes{};
};

struct AssociationFarmsRowsView {
  std::vector<AssociationFarmsRowView> rows{};
};

struct AssociationFarmsMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
