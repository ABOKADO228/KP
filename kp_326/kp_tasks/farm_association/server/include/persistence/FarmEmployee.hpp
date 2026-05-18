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

/// Сущность таблицы farm_employee.
struct FarmEmployeeEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки person_id.
  std::uint64_t personId{};

  /// Значение колонки farm_id.
  std::uint64_t farmId{};

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

  /// Значение колонки employment_contract_number.
  odb::nullable<std::string> employmentContractNumber;

  /// Значение колонки is_primary_workplace.
  odb::nullable<bool> isPrimaryWorkplace;

};

#pragma db object(FarmEmployeeEntity) table("farm_employee")
#pragma db member(FarmEmployeeEntity::id) id auto column("id")
#pragma db member(FarmEmployeeEntity::personId) column("person_id")
#pragma db member(FarmEmployeeEntity::farmId) column("farm_id")
#pragma db member(FarmEmployeeEntity::roleId) column("role_id")
#pragma db member(FarmEmployeeEntity::employmentStatusId) column("employment_status_id")
#pragma db member(FarmEmployeeEntity::hireDate) column("hire_date")
#pragma db member(FarmEmployeeEntity::dismissalDate) column("dismissal_date")
#pragma db member(FarmEmployeeEntity::salary) column("salary")
#pragma db member(FarmEmployeeEntity::employmentContractNumber) column("employment_contract_number")
#pragma db member(FarmEmployeeEntity::isPrimaryWorkplace) column("is_primary_workplace")

} // namespace persistence
} // namespace server
} // namespace fasc
