#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PurchaseRequisitionRowView {
  std::uint64_t id{};
  std::uint64_t farmId{};
  std::uint64_t productId{};
  int quantity{};
  std::optional<double> maxPricePerUnit{};
  std::optional<fasc::server::domain::Date> offerDate{};
  std::optional<fasc::server::domain::Date> requiredDate{};
  std::optional<int> priority{};
  std::optional<fasc::server::domain::Date> validUntil{};
  std::optional<fasc::server::domain::PurchaseRequisitionStatus> status{};
  std::optional<std::string> notes{};
};

struct PurchaseRequisitionRowsView {
  std::vector<PurchaseRequisitionRowView> rows{};
};

struct PurchaseRequisitionMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
