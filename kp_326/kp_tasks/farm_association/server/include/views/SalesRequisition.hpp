#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct SalesRequisitionRowView {
  int id{};
  std::optional<int> farmId{};
  int productId{};
  int quantity{};
  std::optional<double> pricePerUnit{};
  std::string offerDate{};
  std::optional<std::string> validUntil{};
  std::optional<std::string> status{};
  std::optional<std::string> notes{};
};

struct SalesRequisitionRowsView {
  std::vector<SalesRequisitionRowView> rows{};
};

struct SalesRequisitionMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
