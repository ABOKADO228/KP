#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmPlotTypeRowView {
  std::uint64_t id{};
  std::optional<std::string> name{};
  std::optional<std::string> description{};
  int farmPlotLevel{};
  std::optional<std::uint64_t> parentId{};
};

struct FarmPlotTypeRowsView {
  std::vector<FarmPlotTypeRowView> rows{};
};

struct FarmPlotTypeMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
