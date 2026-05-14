#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы association_employee.
struct AssociationEmployeeEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки person_id.
  int personId{};

  /// Значение колонки association_id.
  int associationId{};

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

} // namespace fasc::server::persistence
