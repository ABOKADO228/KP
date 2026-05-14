#pragma once

#include <persistence/FarmAssociation.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_association.
struct FarmAssociationRowView {
  /// Данные строки.
  fasc::server::persistence::FarmAssociationEntity data;
};

/// View списка строк таблицы farm_association.
struct FarmAssociationRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmAssociationEntity> rows;
};

/// View результата изменения таблицы farm_association.
struct FarmAssociationMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
