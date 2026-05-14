#pragma once

#include <persistence/FarmPlotProductionProduct.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_plot_production_product.
struct FarmPlotProductionProductRowView {
  /// Данные строки.
  fasc::server::persistence::FarmPlotProductionProductEntity data;
};

/// View списка строк таблицы farm_plot_production_product.
struct FarmPlotProductionProductRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotProductionProductEntity> rows;
};

/// View результата изменения таблицы farm_plot_production_product.
struct FarmPlotProductionProductMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
