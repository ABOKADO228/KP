#pragma once

#include <persistence/Farm.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы farm.
struct FarmRowView {
  /// Данные строки.
  fasc::server::persistence::FarmEntity data;
};

/// View списка строк таблицы farm.
struct FarmRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmEntity> rows;
};

/// View результата изменения таблицы farm.
struct FarmMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
