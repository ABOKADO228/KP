#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_employee.
struct FarmEmployeeEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки person_id.
  int personId{};

  /// Значение колонки farm_id.
  int farmId{};

  /// Значение колонки role_id.
  int roleId{};

  /// Значение колонки employment_status_id.
  int employmentStatusId{};

  /// Значение колонки hire_date.
  std::string hireDate;

  /// Значение колонки dismissal_date.
  odb::nullable<std::string> dismissalDate;

  /// Значение колонки salary.
  odb::nullable<double> salary;

  /// Значение колонки employment_contract_number.
  odb::nullable<std::string> employmentContractNumber;

  /// Значение колонки is_primary_workplace.
  odb::nullable<int> isPrimaryWorkplace;

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

} // namespace fasc::server::persistence
