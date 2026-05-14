#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы unit.
struct UnitEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки code.
  odb::nullable<std::string> code;

  /// Значение колонки name.
  odb::nullable<std::string> name;

};

#pragma db object(UnitEntity) table("unit")
#pragma db member(UnitEntity::id) id column("id")
#pragma db member(UnitEntity::code) column("code")
#pragma db member(UnitEntity::name) column("name")

} // namespace fasc::server::persistence
