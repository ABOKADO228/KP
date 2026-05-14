#pragma once

#include <persistence/PersonDocument.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы person_document.
struct PersonDocumentRowView {
  /// Данные строки.
  fasc::server::persistence::PersonDocumentEntity data;
};

/// View списка строк таблицы person_document.
struct PersonDocumentRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PersonDocumentEntity> rows;
};

/// View результата изменения таблицы person_document.
struct PersonDocumentMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
