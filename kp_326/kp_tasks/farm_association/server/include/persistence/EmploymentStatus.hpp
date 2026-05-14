#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы employment_status.
struct EmploymentStatusEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки name.
  odb::nullable<fasc::server::domain::EmploymentStatusCode> name;

};

#pragma db object(EmploymentStatusEntity) table("employment_status")
#pragma db member(EmploymentStatusEntity::id) id column("id")
#pragma db member(EmploymentStatusEntity::name) column("name")

} // namespace fasc::server::persistence
