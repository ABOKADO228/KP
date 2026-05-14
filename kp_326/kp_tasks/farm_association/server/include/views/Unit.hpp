#pragma once

#include <persistence/Unit.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы unit.
struct UnitRowView {
  /// Данные строки.
  fasc::server::persistence::UnitEntity data;
};

/// View списка строк таблицы unit.
struct UnitRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::UnitEntity> rows;
};

/// View результата изменения таблицы unit.
struct UnitMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
