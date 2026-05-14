#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm.
struct FarmEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки legal_name.
  odb::nullable<std::string> legalName;

  /// Значение колонки legal_address.
  odb::nullable<std::string> legalAddress;

  /// Значение колонки actual_address.
  odb::nullable<std::string> actualAddress;

  /// Значение колонки inn.
  odb::nullable<std::string> inn;

  /// Значение колонки ogrn.
  odb::nullable<std::string> ogrn;

  /// Значение колонки status.
  odb::nullable<std::string> status;

  /// Значение колонки farm_type.
  odb::nullable<std::string> farmType;

};

#pragma db object(FarmEntity) table("farm")
#pragma db member(FarmEntity::id) id auto column("id")
#pragma db member(FarmEntity::name) column("name")
#pragma db member(FarmEntity::legalName) column("legal_name")
#pragma db member(FarmEntity::legalAddress) column("legal_address")
#pragma db member(FarmEntity::actualAddress) column("actual_address")
#pragma db member(FarmEntity::inn) column("inn")
#pragma db member(FarmEntity::ogrn) column("ogrn")
#pragma db member(FarmEntity::status) column("status")
#pragma db member(FarmEntity::farmType) column("farm_type")

} // namespace fasc::server::persistence
