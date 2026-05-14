#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы association_role.
struct AssociationRoleEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки code.
  odb::nullable<std::string> code;

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

} // namespace fasc::server::persistence
