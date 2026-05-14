#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_owner.
struct FarmOwnerEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки person_id.
  int personId{};

  /// Значение колонки status.
  odb::nullable<std::string> status;

  /// Значение колонки rating.
  odb::nullable<double> rating;

};

#pragma db object(FarmOwnerEntity) table("farm_owner")
#pragma db member(FarmOwnerEntity::id) id auto column("id")
#pragma db member(FarmOwnerEntity::personId) column("person_id")
#pragma db member(FarmOwnerEntity::status) column("status")
#pragma db member(FarmOwnerEntity::rating) column("rating")

} // namespace fasc::server::persistence
