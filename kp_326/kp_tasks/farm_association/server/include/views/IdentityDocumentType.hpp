#pragma once

#include <persistence/IdentityDocumentType.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы identity_document_type.
struct IdentityDocumentTypeRowView {
  /// Данные строки.
  fasc::server::persistence::IdentityDocumentTypeEntity data;
};

/// View списка строк таблицы identity_document_type.
struct IdentityDocumentTypeRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::IdentityDocumentTypeEntity> rows;
};

/// View результата изменения таблицы identity_document_type.
struct IdentityDocumentTypeMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
