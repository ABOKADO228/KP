#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PurchaseOrderItemRowView {
  std::uint64_t id{};
  std::uint64_t purchaseOrderId{};
  std::uint64_t productId{};
  std::optional<double> quantity{};
  std::optional<double> unitPrice{};
  std::optional<double> vatRate{};
  std::optional<fasc::server::domain::CurrencyCode> currency{};
};

struct PurchaseOrderItemRowsView {
  std::vector<PurchaseOrderItemRowView> rows{};
};

struct PurchaseOrderItemMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
