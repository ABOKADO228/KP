#pragma once

#include <persistence/PurchaseOrderItem.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы purchase_order_item.
struct PurchaseOrderItemRowView {
  /// Данные строки.
  fasc::server::persistence::PurchaseOrderItemEntity data;
};

/// View списка строк таблицы purchase_order_item.
struct PurchaseOrderItemRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PurchaseOrderItemEntity> rows;
};

/// View результата изменения таблицы purchase_order_item.
struct PurchaseOrderItemMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
