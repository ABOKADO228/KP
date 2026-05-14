#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы product_type.
struct ProductTypeEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки parent_id.
  odb::nullable<int> parentId;

  /// Значение колонки sku.
  odb::nullable<std::string> sku;

  /// Значение колонки product_level.
  int productLevel{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки description.
  odb::nullable<std::string> description;

};

#pragma db object(ProductTypeEntity) table("product_type")
#pragma db member(ProductTypeEntity::id) id column("id")
#pragma db member(ProductTypeEntity::parentId) column("parent_id")
#pragma db member(ProductTypeEntity::sku) column("sku")
#pragma db member(ProductTypeEntity::productLevel) column("product_level")
#pragma db member(ProductTypeEntity::name) column("name")
#pragma db member(ProductTypeEntity::description) column("description")

} // namespace fasc::server::persistence
