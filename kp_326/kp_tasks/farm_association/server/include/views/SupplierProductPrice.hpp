#pragma once

#include <persistence/SupplierProductPrice.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы supplier_product_price.
struct SupplierProductPriceRowView {
  /// Данные строки.
  fasc::server::persistence::SupplierProductPriceEntity data;
};

/// View списка строк таблицы supplier_product_price.
struct SupplierProductPriceRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::SupplierProductPriceEntity> rows;
};

/// View результата изменения таблицы supplier_product_price.
struct SupplierProductPriceMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
