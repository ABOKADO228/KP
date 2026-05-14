#pragma once

#include <persistence/FarmOwner.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm_owner.
struct FarmOwnerRowView {
  /// Данные строки.
  fasc::server::persistence::FarmOwnerEntity data;
};

/// View списка строк таблицы farm_owner.
struct FarmOwnerRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmOwnerEntity> rows;
};

/// View результата изменения таблицы farm_owner.
struct FarmOwnerMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
