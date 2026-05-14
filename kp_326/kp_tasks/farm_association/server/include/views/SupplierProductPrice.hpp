#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct SupplierProductPriceRowView {
  int id{};
  int supplierId{};
  int productId{};
  std::optional<double> purchasePrice{};
  std::string validFrom{};
  std::optional<std::string> validUntil{};
};

struct SupplierProductPriceRowsView {
  std::vector<SupplierProductPriceRowView> rows{};
};

struct SupplierProductPriceMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
