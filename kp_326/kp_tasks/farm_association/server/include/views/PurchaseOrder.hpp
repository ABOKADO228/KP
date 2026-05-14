#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PurchaseOrderRowView {
  std::uint64_t id{};
  std::uint64_t associationId{};
  std::uint64_t supplierId{};
  std::optional<std::string> deliveryAddress{};
  fasc::server::domain::Date orderDate{};
  std::optional<fasc::server::domain::Date> expectedDeliveryDate{};
  std::optional<fasc::server::domain::PurchaseOrderStatus> status{};
  std::optional<double> totalAmount{};
  std::optional<fasc::server::domain::Date> receivedAt{};
  std::optional<std::uint64_t> createdBy{};
};

struct PurchaseOrderRowsView {
  std::vector<PurchaseOrderRowView> rows{};
};

struct PurchaseOrderMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
