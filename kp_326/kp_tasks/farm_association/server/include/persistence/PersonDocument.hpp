#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы person_document.
struct PersonDocumentEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки person_id.
  int personId{};

  /// Значение колонки document_type_id.
  int documentTypeId{};

  /// Значение колонки document_number.
  odb::nullable<std::string> documentNumber;

  /// Значение колонки issued_by.
  odb::nullable<std::string> issuedBy;

  /// Значение колонки issued_date.
  odb::nullable<std::string> issuedDate;

  /// Значение колонки expiration_date.
  odb::nullable<std::string> expirationDate;

  /// Значение колонки is_primary.
  odb::nullable<int> isPrimary;

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
