#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotRowView {
  int id{};
  std::optional<int> farmPlotTypeId{};
  std::optional<std::string> name{};
  std::optional<double> area{};
  std::optional<std::string> location{};
  std::optional<std::string> cadastralNumber{};
  std::optional<std::string> status{};
};

struct FarmPlotRowsView {
  std::vector<FarmPlotRowView> rows{};
};

struct FarmPlotMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
