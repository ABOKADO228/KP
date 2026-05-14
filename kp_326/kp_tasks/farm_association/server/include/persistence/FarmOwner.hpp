#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_owner.
struct FarmOwnerEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки person_id.
  std::uint64_t personId{};

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::FarmOwnerStatus> status;

  /// Значение колонки rating.
  odb::nullable<double> rating;

};

#pragma db object(FarmOwnerEntity) table("farm_owner")
#pragma db member(FarmOwnerEntity::id) id auto column("id")
#pragma db member(FarmOwnerEntity::personId) column("person_id")
#pragma db member(FarmOwnerEntity::status) column("status")
#pragma db member(FarmOwnerEntity::rating) column("rating")

} // namespace fasc::server::persistence
