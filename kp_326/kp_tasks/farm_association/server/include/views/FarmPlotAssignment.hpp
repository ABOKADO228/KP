#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotAssignmentRowView {
  int farmId{};
  int farmPlotId{};
  std::optional<std::string> status{};
  std::optional<std::string> notes{};
};

struct FarmPlotAssignmentRowsView {
  std::vector<FarmPlotAssignmentRowView> rows{};
};

struct FarmPlotAssignmentMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
