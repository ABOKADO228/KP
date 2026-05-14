#pragma once

#include <persistence/FarmPlotType.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_plot_type.
struct FarmPlotTypeRowView {
  /// Данные строки.
  fasc::server::persistence::FarmPlotTypeEntity data;
};

/// View списка строк таблицы farm_plot_type.
struct FarmPlotTypeRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotTypeEntity> rows;
};

/// View результата изменения таблицы farm_plot_type.
struct FarmPlotTypeMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
