#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/PersonDocument.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы person_document.
struct PersonDocumentKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы person_document.
struct PersonDocumentCreateDto {
  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки document_type_id.
  std::optional<std::uint64_t> documentTypeId;

  /// Значение колонки document_number.
  std::optional<std::string> documentNumber;

  /// Значение колонки issued_by.
  std::optional<std::string> issuedBy;

  /// Значение колонки issued_date.
  std::optional<fasc::server::domain::Date> issuedDate;

  /// Значение колонки expiration_date.
  std::optional<fasc::server::domain::Date> expirationDate;

  /// Значение колонки is_primary.
  std::optional<bool> isPrimary;

};

/// DTO обновления строки таблицы person_document.
struct PersonDocumentUpdateDto {
  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки document_type_id.
  std::optional<std::uint64_t> documentTypeId;

  /// Значение колонки document_number.
  std::optional<std::string> documentNumber;

  /// Значение колонки issued_by.
  std::optional<std::string> issuedBy;

  /// Значение колонки issued_date.
  std::optional<fasc::server::domain::Date> issuedDate;

  /// Значение колонки expiration_date.
  std::optional<fasc::server::domain::Date> expirationDate;

  /// Значение колонки is_primary.
  std::optional<bool> isPrimary;

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
