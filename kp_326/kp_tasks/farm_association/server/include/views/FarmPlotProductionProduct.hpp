#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotProductionProductRowView {
  std::uint64_t productId{};
  std::uint64_t farmPlotId{};
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
