#pragma once

#include <persistence/PurchaseRequisition.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы purchase_requisition.
struct PurchaseRequisitionRowView {
  /// Данные строки.
  fasc::server::persistence::PurchaseRequisitionEntity data;
};

/// View списка строк таблицы purchase_requisition.
struct PurchaseRequisitionRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PurchaseRequisitionEntity> rows;
};

/// View результата изменения таблицы purchase_requisition.
struct PurchaseRequisitionMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
