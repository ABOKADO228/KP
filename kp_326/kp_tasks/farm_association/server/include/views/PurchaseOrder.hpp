#pragma once

#include <persistence/PurchaseOrder.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы purchase_order.
struct PurchaseOrderRowView {
  /// Данные строки.
  fasc::server::persistence::PurchaseOrderEntity data;
};

/// View списка строк таблицы purchase_order.
struct PurchaseOrderRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PurchaseOrderEntity> rows;
};

/// View результата изменения таблицы purchase_order.
struct PurchaseOrderMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
