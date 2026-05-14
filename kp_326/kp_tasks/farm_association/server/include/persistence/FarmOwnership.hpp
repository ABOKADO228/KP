#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_ownership.
struct FarmOwnershipEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки farm_id.
  int farmId{};

  /// Значение колонки farm_owner_id.
  int farmOwnerId{};

  /// Значение колонки ownership_percentage.
  odb::nullable<double> ownershipPercentage;

  /// Значение колонки started_at.
  std::string startedAt;

  /// Значение колонки ended_at.
  odb::nullable<std::string> endedAt;

};

#pragma db object(FarmOwnershipEntity) table("farm_ownership")
#pragma db member(FarmOwnershipEntity::id) id auto column("id")
#pragma db member(FarmOwnershipEntity::farmId) column("farm_id")
#pragma db member(FarmOwnershipEntity::farmOwnerId) column("farm_owner_id")
#pragma db member(FarmOwnershipEntity::ownershipPercentage) column("ownership_percentage")
#pragma db member(FarmOwnershipEntity::startedAt) column("started_at")
#pragma db member(FarmOwnershipEntity::endedAt) column("ended_at")

} // namespace fasc::server::persistence
