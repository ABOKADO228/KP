#pragma once

#include <persistence/Product.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы product.
struct ProductRowView {
  /// Данные строки.
  fasc::server::persistence::ProductEntity data;
};

/// View списка строк таблицы product.
struct ProductRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::ProductEntity> rows;
};

/// View результата изменения таблицы product.
struct ProductMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
