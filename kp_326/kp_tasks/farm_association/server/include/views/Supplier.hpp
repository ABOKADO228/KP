#pragma once

#include <persistence/Supplier.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы supplier.
struct SupplierRowView {
  /// Данные строки.
  fasc::server::persistence::SupplierEntity data;
};

/// View списка строк таблицы supplier.
struct SupplierRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::SupplierEntity> rows;
};

/// View результата изменения таблицы supplier.
struct SupplierMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
