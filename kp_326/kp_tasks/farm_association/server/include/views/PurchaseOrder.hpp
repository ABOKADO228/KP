#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PurchaseOrderRowView {
  int id{};
  int associationId{};
  int supplierId{};
  std::optional<std::string> deliveryAddress{};
  std::string orderDate{};
  std::optional<std::string> expectedDeliveryDate{};
  std::optional<std::string> status{};
  std::optional<double> totalAmount{};
  std::optional<std::string> receivedAt{};
  std::optional<int> createdBy{};
};

struct PurchaseOrderRowsView {
  std::vector<PurchaseOrderRowView> rows{};
};

struct PurchaseOrderMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
