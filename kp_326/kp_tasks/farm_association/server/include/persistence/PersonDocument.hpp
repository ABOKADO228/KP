#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы person_document.
struct PersonDocumentEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки person_id.
  std::uint64_t personId{};

  /// Значение колонки document_type_id.
  std::uint64_t documentTypeId{};

  /// Значение колонки document_number.
  odb::nullable<std::string> documentNumber;

  /// Значение колонки issued_by.
  odb::nullable<std::string> issuedBy;

  /// Значение колонки issued_date.
  odb::nullable<fasc::server::domain::Date> issuedDate;

  /// Значение колонки expiration_date.
  odb::nullable<fasc::server::domain::Date> expirationDate;

  /// Значение колонки is_primary.
  odb::nullable<bool> isPrimary;

};

#pragma db object(PersonDocumentEntity) table("person_document")
#pragma db member(PersonDocumentEntity::id) id auto column("id")
#pragma db member(PersonDocumentEntity::personId) column("person_id")
#pragma db member(PersonDocumentEntity::documentTypeId) column("document_type_id")
#pragma db member(PersonDocumentEntity::documentNumber) column("document_number")
#pragma db member(PersonDocumentEntity::issuedBy) column("issued_by")
#pragma db member(PersonDocumentEntity::issuedDate) column("issued_date")
#pragma db member(PersonDocumentEntity::expirationDate) column("expiration_date")
#pragma db member(PersonDocumentEntity::isPrimary) column("is_primary")

} // namespace fasc::server::persistence
