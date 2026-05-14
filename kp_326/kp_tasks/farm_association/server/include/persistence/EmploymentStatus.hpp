#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы employment_status.
struct EmploymentStatusEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки name.
  odb::nullable<std::string> name;

};

#pragma db object(EmploymentStatusEntity) table("employment_status")
#pragma db member(EmploymentStatusEntity::id) id column("id")
#pragma db member(EmploymentStatusEntity::name) column("name")

} // namespace fasc::server::persistence
