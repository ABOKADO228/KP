#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct SalesRequisitionRowView {
  std::uint64_t id{};
  std::optional<std::uint64_t> farmId{};
  std::uint64_t productId{};
  int quantity{};
  std::optional<double> pricePerUnit{};
  fasc::server::domain::Date offerDate{};
  std::optional<fasc::server::domain::Date> validUntil{};
  std::optional<fasc::server::domain::SalesRequisitionStatus> status{};
  std::optional<std::string> notes{};
};

struct SalesRequisitionRowsView {
  std::vector<SalesRequisitionRowView> rows{};
};

struct SalesRequisitionMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
