#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotConsumptionProductRowView {
  int productId{};
  int farmPlotId{};
  int quantity{};
  int consumptionNow{};
};

struct FarmPlotConsumptionProductRowsView {
  std::vector<FarmPlotConsumptionProductRowView> rows{};
};

struct FarmPlotConsumptionProductMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
