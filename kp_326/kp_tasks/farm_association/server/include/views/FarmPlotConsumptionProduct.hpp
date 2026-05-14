#pragma once

#include <persistence/FarmPlotConsumptionProduct.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductRowView {
  /// Данные строки.
  fasc::server::persistence::FarmPlotConsumptionProductEntity data;
};

/// View списка строк таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotConsumptionProductEntity> rows;
};

/// View результата изменения таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
