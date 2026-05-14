#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmOwnerRowView {
  int id{};
  int personId{};
  std::optional<std::string> status{};
  std::optional<double> rating{};
};

struct FarmOwnerRowsView {
  std::vector<FarmOwnerRowView> rows{};
};

struct FarmOwnerMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
