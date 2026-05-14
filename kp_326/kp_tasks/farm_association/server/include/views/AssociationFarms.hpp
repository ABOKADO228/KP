#pragma once

#include <persistence/AssociationFarms.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы association_farms.
struct AssociationFarmsRowView {
  /// Данные строки.
  fasc::server::persistence::AssociationFarmsEntity data;
};

/// View списка строк таблицы association_farms.
struct AssociationFarmsRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationFarmsEntity> rows;
};

/// View результата изменения таблицы association_farms.
struct AssociationFarmsMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
