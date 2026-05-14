#pragma once

#include <persistence/ProductType.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы product_type.
struct ProductTypeRowView {
  /// Данные строки.
  fasc::server::persistence::ProductTypeEntity data;
};

/// View списка строк таблицы product_type.
struct ProductTypeRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::ProductTypeEntity> rows;
};

/// View результата изменения таблицы product_type.
struct ProductTypeMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
