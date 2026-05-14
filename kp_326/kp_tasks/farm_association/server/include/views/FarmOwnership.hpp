#pragma once

#include <persistence/FarmOwnership.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_ownership.
struct FarmOwnershipRowView {
  /// Данные строки.
  fasc::server::persistence::FarmOwnershipEntity data;
};

/// View списка строк таблицы farm_ownership.
struct FarmOwnershipRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmOwnershipEntity> rows;
};

/// View результата изменения таблицы farm_ownership.
struct FarmOwnershipMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
