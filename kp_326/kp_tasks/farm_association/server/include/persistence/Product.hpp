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

/// Сущность таблицы product.
struct ProductEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки type_id.
  std::uint64_t typeId{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки unit_id.
  odb::nullable<unsigned long long> unitId;

};

#pragma db object(ProductEntity) table("product")
#pragma db member(ProductEntity::id) id auto column("id")
#pragma db member(ProductEntity::typeId) column("type_id")
#pragma db member(ProductEntity::name) column("name")
#pragma db member(ProductEntity::unitId) column("unit_id")

} // namespace persistence
} // namespace server
} // namespace fasc
