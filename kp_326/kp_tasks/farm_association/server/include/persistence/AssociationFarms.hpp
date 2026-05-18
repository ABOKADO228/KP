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

/// Составной ключ таблицы association_farms.
struct AssociationFarmsKey {
  AssociationFarmsKey() = default;
  AssociationFarmsKey(std::uint64_t farmIdValue, std::uint64_t associationIdValue)
      : farmId(farmIdValue), associationId(associationIdValue) {}

  std::uint64_t farmId{};
  std::uint64_t associationId{};
};

/// Сущность таблицы association_farms.
struct AssociationFarmsEntity {
  /// Значение колонки farm_id.
  std::uint64_t farmId{};

  /// Значение колонки association_id.
  std::uint64_t associationId{};

  /// Значение колонки join_date.
  fasc::server::domain::Date joinDate;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::AssociationFarmsStatus> status;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

  AssociationFarmsKey id() const {
    return AssociationFarmsKey{farmId, associationId};
  }

  void id(const AssociationFarmsKey& key) {
    farmId = key.farmId;
    associationId = key.associationId;
  }

};

#pragma db value(AssociationFarmsKey)
#pragma db member(AssociationFarmsKey::farmId) column("farm_id")
#pragma db member(AssociationFarmsKey::associationId) column("association_id")

#pragma db object(AssociationFarmsEntity) table("association_farms")
#pragma db member(AssociationFarmsEntity::id) virtual(AssociationFarmsKey) access(id) id column("")
#pragma db member(AssociationFarmsEntity::farmId) transient
#pragma db member(AssociationFarmsEntity::associationId) transient
#pragma db member(AssociationFarmsEntity::joinDate) column("join_date")
#pragma db member(AssociationFarmsEntity::status) column("status")
#pragma db member(AssociationFarmsEntity::notes) column("notes")

} // namespace persistence
} // namespace server
} // namespace fasc
