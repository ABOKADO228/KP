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

/// Сущность таблицы unit.
struct UnitEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки code.
  odb::nullable<fasc::server::domain::UnitCode> code;

  /// Значение колонки name.
  odb::nullable<std::string> name;

};

#pragma db object(UnitEntity) table("unit")
#pragma db member(UnitEntity::id) id column("id")
#pragma db member(UnitEntity::code) column("code")
#pragma db member(UnitEntity::name) column("name")

} // namespace persistence
} // namespace server
} // namespace fasc
