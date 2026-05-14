#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmOwnerRowView {
  std::uint64_t id{};
  std::uint64_t personId{};
  std::optional<fasc::server::domain::FarmOwnerStatus> status{};
  std::optional<double> rating{};
};

struct FarmOwnerRowsView {
  std::vector<FarmOwnerRowView> rows{};
};

struct FarmOwnerMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
