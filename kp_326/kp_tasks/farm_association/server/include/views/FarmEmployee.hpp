#pragma once

#include <persistence/FarmEmployee.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_employee.
struct FarmEmployeeRowView {
  /// Данные строки.
  fasc::server::persistence::FarmEmployeeEntity data;
};

/// View списка строк таблицы farm_employee.
struct FarmEmployeeRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmEmployeeEntity> rows;
};

/// View результата изменения таблицы farm_employee.
struct FarmEmployeeMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
