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

/// Сущность таблицы farm_association.
struct FarmAssociationEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки inn.
  odb::nullable<std::string> inn;

  /// Значение колонки ogrn.
  odb::nullable<std::string> ogrn;

  /// Значение колонки name.
  odb::nullable<std::string> name;

  /// Значение колонки legal_address.
  odb::nullable<std::string> legalAddress;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::FarmAssociationStatus> status;

};

#pragma db object(FarmAssociationEntity) table("farm_association")
#pragma db member(FarmAssociationEntity::id) id auto column("id")
#pragma db member(FarmAssociationEntity::inn) column("inn")
#pragma db member(FarmAssociationEntity::ogrn) column("ogrn")
#pragma db member(FarmAssociationEntity::name) column("name")
#pragma db member(FarmAssociationEntity::legalAddress) column("legal_address")
#pragma db member(FarmAssociationEntity::status) column("status")

} // namespace persistence
} // namespace server
} // namespace fasc
