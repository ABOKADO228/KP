#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PurchaseOrderItemRowView {
  int id{};
  int purchaseOrderId{};
  int productId{};
  std::optional<double> quantity{};
  std::optional<double> unitPrice{};
  std::optional<double> vatRate{};
  std::optional<std::string> currency{};
};

struct PurchaseOrderItemRowsView {
  std::vector<PurchaseOrderItemRowView> rows{};
};

struct PurchaseOrderItemMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
