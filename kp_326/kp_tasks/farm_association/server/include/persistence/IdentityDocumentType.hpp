#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#ifndef ODB_COMPILER
#include <persistence/DomainValueTraits.hpp>
#endif

#include <string>

namespace fasc {
namespace server {
namespace persistence {

/// Сущность таблицы identity_document_type.
struct IdentityDocumentTypeEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки code.
  odb::nullable<fasc::server::domain::IdentityDocumentTypeCode> code;

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

} // namespace persistence
} // namespace server
} // namespace fasc
