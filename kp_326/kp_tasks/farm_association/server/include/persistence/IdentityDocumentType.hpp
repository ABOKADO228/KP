#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы identity_document_type.
struct IdentityDocumentTypeEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки code.
  odb::nullable<std::string> code;

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки description.
  odb::nullable<std::string> description;

};

#pragma db object(IdentityDocumentTypeEntity) table("identity_document_type")
#pragma db member(IdentityDocumentTypeEntity::id) id column("id")
#pragma db member(IdentityDocumentTypeEntity::code) column("code")
#pragma db member(IdentityDocumentTypeEntity::name) column("name")
#pragma db member(IdentityDocumentTypeEntity::description) column("description")

} // namespace fasc::server::persistence
