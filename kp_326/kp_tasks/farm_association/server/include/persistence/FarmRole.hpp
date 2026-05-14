#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_role.
struct FarmRoleEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки description.
  odb::nullable<std::string> description;

};

#pragma db object(FarmRoleEntity) table("farm_role")
#pragma db member(FarmRoleEntity::id) id column("id")
#pragma db member(FarmRoleEntity::name) column("name")
#pragma db member(FarmRoleEntity::description) column("description")

} // namespace fasc::server::persistence
