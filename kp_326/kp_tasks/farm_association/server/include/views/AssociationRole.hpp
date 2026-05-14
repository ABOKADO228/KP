#pragma once

#include <persistence/AssociationRole.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы association_role.
struct AssociationRoleRowView {
  /// Данные строки.
  fasc::server::persistence::AssociationRoleEntity data;
};

/// View списка строк таблицы association_role.
struct AssociationRoleRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationRoleEntity> rows;
};

/// View результата изменения таблицы association_role.
struct AssociationRoleMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
