#pragma once

#include <persistence/AssociationEmployee.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы association_employee.
struct AssociationEmployeeRowView {
  /// Данные строки.
  fasc::server::persistence::AssociationEmployeeEntity data;
};

/// View списка строк таблицы association_employee.
struct AssociationEmployeeRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationEmployeeEntity> rows;
};

/// View результата изменения таблицы association_employee.
struct AssociationEmployeeMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
