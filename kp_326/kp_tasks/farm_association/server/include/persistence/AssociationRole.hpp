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

/// Сущность таблицы association_role.
struct AssociationRoleEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки code.
  odb::nullable<fasc::server::domain::AssociationRoleCode> code;

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки description.
  odb::nullable<std::string> description;

};

#pragma db object(AssociationRoleEntity) table("association_role")
#pragma db member(AssociationRoleEntity::id) id column("id")
#pragma db member(AssociationRoleEntity::code) column("code")
#pragma db member(AssociationRoleEntity::name) column("name")
#pragma db member(AssociationRoleEntity::description) column("description")

} // namespace persistence
} // namespace server
} // namespace fasc
