#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotAssignmentRowView {
  std::uint64_t farmId{};
  std::uint64_t farmPlotId{};
  std::optional<fasc::server::domain::FarmPlotAssignmentStatus> status{};
  std::optional<std::string> notes{};
};

struct FarmPlotAssignmentRowsView {
  std::vector<FarmPlotAssignmentRowView> rows{};
};

struct FarmPlotAssignmentMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
