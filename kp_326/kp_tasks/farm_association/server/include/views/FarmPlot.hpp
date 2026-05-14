#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotRowView {
  std::uint64_t id{};
  std::optional<std::uint64_t> farmPlotTypeId{};
  std::optional<std::string> name{};
  std::optional<double> area{};
  std::optional<std::string> location{};
  std::optional<std::string> cadastralNumber{};
  std::optional<fasc::server::domain::FarmPlotStatus> status{};
};

struct FarmPlotRowsView {
  std::vector<FarmPlotRowView> rows{};
};

struct FarmPlotMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
