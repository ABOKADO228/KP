#pragma once

#include <persistence/SalesRequisition.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы sales_requisition.
struct SalesRequisitionRowView {
  /// Данные строки.
  fasc::server::persistence::SalesRequisitionEntity data;
};

/// View списка строк таблицы sales_requisition.
struct SalesRequisitionRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::SalesRequisitionEntity> rows;
};

/// View результата изменения таблицы sales_requisition.
struct SalesRequisitionMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
