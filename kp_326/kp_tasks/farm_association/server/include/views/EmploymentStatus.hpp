#pragma once

#include <persistence/EmploymentStatus.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы employment_status.
struct EmploymentStatusRowView {
  /// Данные строки.
  fasc::server::persistence::EmploymentStatusEntity data;
};

/// View списка строк таблицы employment_status.
struct EmploymentStatusRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::EmploymentStatusEntity> rows;
};

/// View результата изменения таблицы employment_status.
struct EmploymentStatusMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
