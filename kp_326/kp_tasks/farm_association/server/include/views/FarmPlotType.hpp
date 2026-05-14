#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotTypeRowView {
  int id{};
  std::optional<std::string> name{};
  std::optional<std::string> description{};
  int farmPlotLevel{};
  std::optional<int> parentId{};
};

struct FarmPlotTypeRowsView {
  std::vector<FarmPlotTypeRowView> rows{};
};

struct FarmPlotTypeMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
