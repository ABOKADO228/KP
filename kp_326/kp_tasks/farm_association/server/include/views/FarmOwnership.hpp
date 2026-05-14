#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmOwnershipRowView {
  int id{};
  int farmId{};
  int farmOwnerId{};
  std::optional<double> ownershipPercentage{};
  std::string startedAt{};
  std::optional<std::string> endedAt{};
};

struct FarmOwnershipRowsView {
  std::vector<FarmOwnershipRowView> rows{};
};

struct FarmOwnershipMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
