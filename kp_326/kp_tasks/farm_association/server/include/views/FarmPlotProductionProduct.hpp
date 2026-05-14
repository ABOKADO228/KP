#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotProductionProductRowView {
  int productId{};
  int farmPlotId{};
  int quantity{};
  int productionNow{};
};

struct FarmPlotProductionProductRowsView {
  std::vector<FarmPlotProductionProductRowView> rows{};
};

struct FarmPlotProductionProductMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
