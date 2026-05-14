#pragma once

#include <persistence/EmployeePlotAssignment.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы employee_plot_assignment.
struct EmployeePlotAssignmentRowView {
  /// Данные строки.
  fasc::server::persistence::EmployeePlotAssignmentEntity data;
};

/// View списка строк таблицы employee_plot_assignment.
struct EmployeePlotAssignmentRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::EmployeePlotAssignmentEntity> rows;
};

/// View результата изменения таблицы employee_plot_assignment.
struct EmployeePlotAssignmentMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
