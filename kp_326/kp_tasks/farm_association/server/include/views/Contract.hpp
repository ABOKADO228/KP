#pragma once

#include <persistence/Contract.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы contract.
struct ContractRowView {
  /// Данные строки.
  fasc::server::persistence::ContractEntity data;
};

/// View списка строк таблицы contract.
struct ContractRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::ContractEntity> rows;
};

/// View результата изменения таблицы contract.
struct ContractMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
