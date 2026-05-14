#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct SupplierProductPriceRowView {
  std::uint64_t id{};
  std::uint64_t supplierId{};
  std::uint64_t productId{};
  std::optional<double> purchasePrice{};
  fasc::server::domain::Date validFrom{};
  std::optional<fasc::server::domain::Date> validUntil{};
};

struct SupplierProductPriceRowsView {
  std::vector<SupplierProductPriceRowView> rows{};
};

struct SupplierProductPriceMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
