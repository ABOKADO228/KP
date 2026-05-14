#pragma once

#include <persistence/FarmPlot.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_plot.
struct FarmPlotRowView {
  /// Данные строки.
  fasc::server::persistence::FarmPlotEntity data;
};

/// View списка строк таблицы farm_plot.
struct FarmPlotRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotEntity> rows;
};

/// View результата изменения таблицы farm_plot.
struct FarmPlotMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
