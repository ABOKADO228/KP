#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/IdentityDocumentType.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы identity_document_type.
struct IdentityDocumentTypeKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы identity_document_type.
struct IdentityDocumentTypeCreateDto {
  /// Значение колонки id.
  std::optional<std::uint64_t> id;

  /// Значение колонки code.
  std::optional<fasc::server::domain::IdentityDocumentTypeCode> code;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO обновления строки таблицы identity_document_type.
struct IdentityDocumentTypeUpdateDto {
  /// Значение колонки code.
  std::optional<fasc::server::domain::IdentityDocumentTypeCode> code;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO строки таблицы identity_document_type.
struct IdentityDocumentTypeRowDto {
  /// Данные строки.
  fasc::server::persistence::IdentityDocumentTypeEntity data;
};

/// DTO списка строк таблицы identity_document_type.
struct IdentityDocumentTypeRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::IdentityDocumentTypeEntity> rows;
};

/// DTO результата изменения таблицы identity_document_type.
struct IdentityDocumentTypeMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
