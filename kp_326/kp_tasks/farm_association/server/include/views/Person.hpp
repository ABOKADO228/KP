#pragma once

#include <persistence/Person.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы person.
struct PersonRowView {
  /// Данные строки.
  fasc::server::persistence::PersonEntity data;
};

/// View списка строк таблицы person.
struct PersonRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PersonEntity> rows;
};

/// View результата изменения таблицы person.
struct PersonMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
