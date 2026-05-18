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

/// Сущность таблицы association_employee.
struct AssociationEmployeeEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки person_id.
  std::uint64_t personId{};

  /// Значение колонки association_id.
  std::uint64_t associationId{};

  /// Значение колонки role_id.
  std::uint64_t roleId{};

  /// Значение колонки employment_status_id.
  std::uint64_t employmentStatusId{};

  /// Значение колонки hire_date.
  fasc::server::domain::Date hireDate;

  /// Значение колонки dismissal_date.
  odb::nullable<fasc::server::domain::Date> dismissalDate;

  /// Значение колонки salary.
  odb::nullable<double> salary;

  /// Значение колонки contract_number.
  odb::nullable<std::string> contractNumber;

};

#pragma db object(AssociationEmployeeEntity) table("association_employee")
#pragma db member(AssociationEmployeeEntity::id) id auto column("id")
#pragma db member(AssociationEmployeeEntity::personId) column("person_id")
#pragma db member(AssociationEmployeeEntity::associationId) column("association_id")
#pragma db member(AssociationEmployeeEntity::roleId) column("role_id")
#pragma db member(AssociationEmployeeEntity::employmentStatusId) column("employment_status_id")
#pragma db member(AssociationEmployeeEntity::hireDate) column("hire_date")
#pragma db member(AssociationEmployeeEntity::dismissalDate) column("dismissal_date")
#pragma db member(AssociationEmployeeEntity::salary) column("salary")
#pragma db member(AssociationEmployeeEntity::contractNumber) column("contract_number")

} // namespace persistence
} // namespace server
} // namespace fasc
