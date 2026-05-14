#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы product.
struct ProductEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки type_id.
  std::uint64_t typeId{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки unit_id.
  odb::nullable<std::uint64_t> unitId;

};

#pragma db object(ProductEntity) table("product")
#pragma db member(ProductEntity::id) id auto column("id")
#pragma db member(ProductEntity::typeId) column("type_id")
#pragma db member(ProductEntity::name) column("name")
#pragma db member(ProductEntity::unitId) column("unit_id")

} // namespace fasc::server::persistence
