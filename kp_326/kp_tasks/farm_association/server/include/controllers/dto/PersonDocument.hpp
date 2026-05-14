#pragma once

#include <persistence/PersonDocument.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы person_document.
struct PersonDocumentKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы person_document.
struct PersonDocumentCreateDto {
  /// Значение колонки person_id.
  std::optional<int> personId;

  /// Значение колонки document_type_id.
  std::optional<int> documentTypeId;

  /// Значение колонки document_number.
  std::optional<std::string> documentNumber;

  /// Значение колонки issued_by.
  std::optional<std::string> issuedBy;

  /// Значение колонки issued_date.
  std::optional<std::string> issuedDate;

  /// Значение колонки expiration_date.
  std::optional<std::string> expirationDate;

  /// Значение колонки is_primary.
  std::optional<int> isPrimary;

};

/// DTO обновления строки таблицы person_document.
struct PersonDocumentUpdateDto {
  /// Значение колонки person_id.
  std::optional<int> personId;

  /// Значение колонки document_type_id.
  std::optional<int> documentTypeId;

  /// Значение колонки document_number.
  std::optional<std::string> documentNumber;

  /// Значение колонки issued_by.
  std::optional<std::string> issuedBy;

  /// Значение колонки issued_date.
  std::optional<std::string> issuedDate;

  /// Значение колонки expiration_date.
  std::optional<std::string> expirationDate;

  /// Значение колонки is_primary.
  std::optional<int> isPrimary;

};

/// DTO строки таблицы person_document.
struct PersonDocumentRowDto {
  /// Данные строки.
  fasc::server::persistence::PersonDocumentEntity data;
};

/// DTO списка строк таблицы person_document.
struct PersonDocumentRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PersonDocumentEntity> rows;
};

/// DTO результата изменения таблицы person_document.
struct PersonDocumentMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
