#pragma once

#include <persistence/FarmRole.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_role.
struct FarmRoleRowView {
  /// Данные строки.
  fasc::server::persistence::FarmRoleEntity data;
};

/// View списка строк таблицы farm_role.
struct FarmRoleRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmRoleEntity> rows;
};

/// View результата изменения таблицы farm_role.
struct FarmRoleMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
