#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы association_farms.
struct AssociationFarmsEntity {
  /// Значение колонки farm_id.
  int farmId{};

  /// Значение колонки association_id.
  int associationId{};

  /// Значение колонки join_date.
  std::string joinDate;

  /// Значение колонки status.
  odb::nullable<std::string> status;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

};

#pragma db object(AssociationFarmsEntity) table("association_farms")
#pragma db member(AssociationFarmsEntity::farmId) column("farm_id")
#pragma db member(AssociationFarmsEntity::associationId) column("association_id")
#pragma db member(AssociationFarmsEntity::joinDate) column("join_date")
#pragma db member(AssociationFarmsEntity::status) column("status")
#pragma db member(AssociationFarmsEntity::notes) column("notes")

} // namespace fasc::server::persistence
