#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotConsumptionProductRowView {
  std::uint64_t productId{};
  std::uint64_t farmPlotId{};
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
