#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmOwnershipRowView {
  std::uint64_t id{};
  std::uint64_t farmId{};
  std::uint64_t farmOwnerId{};
  std::optional<double> ownershipPercentage{};
  fasc::server::domain::Date startedAt{};
  std::optional<fasc::server::domain::Date> endedAt{};
};

struct FarmOwnershipRowsView {
  std::vector<FarmOwnershipRowView> rows{};
};

struct FarmOwnershipMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
