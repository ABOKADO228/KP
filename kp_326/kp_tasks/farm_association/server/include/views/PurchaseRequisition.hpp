#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PurchaseRequisitionRowView {
  int id{};
  int farmId{};
  int productId{};
  int quantity{};
  std::optional<double> maxPricePerUnit{};
  std::optional<std::string> offerDate{};
  std::optional<std::string> requiredDate{};
  std::optional<int> priority{};
  std::optional<std::string> validUntil{};
  std::optional<std::string> status{};
  std::optional<std::string> notes{};
};

struct PurchaseRequisitionRowsView {
  std::vector<PurchaseRequisitionRowView> rows{};
};

struct PurchaseRequisitionMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
