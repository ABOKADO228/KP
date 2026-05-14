#pragma once

#include <persistence/FarmPlotAssignment.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_plot_assignment.
struct FarmPlotAssignmentRowView {
  /// Данные строки.
  fasc::server::persistence::FarmPlotAssignmentEntity data;
};

/// View списка строк таблицы farm_plot_assignment.
struct FarmPlotAssignmentRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotAssignmentEntity> rows;
};

/// View результата изменения таблицы farm_plot_assignment.
struct FarmPlotAssignmentMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
