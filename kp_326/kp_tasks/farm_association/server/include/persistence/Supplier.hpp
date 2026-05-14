#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы supplier.
struct SupplierEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки legal_address.
  odb::nullable<std::string> legalAddress;

  /// Значение колонки status.
  odb::nullable<std::string> status;

};

#pragma db object(SupplierEntity) table("supplier")
#pragma db member(SupplierEntity::id) id auto column("id")
#pragma db member(SupplierEntity::name) column("name")
#pragma db member(SupplierEntity::legalAddress) column("legal_address")
#pragma db member(SupplierEntity::status) column("status")

} // namespace fasc::server::persistence
