#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы employee_plot_assignment.
struct EmployeePlotAssignmentEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки farm_employee_id.
  int farmEmployeeId{};

  /// Значение колонки farm_plot_id.
  int farmPlotId{};

  /// Значение колонки assignment_type.
  odb::nullable<std::string> assignmentType;

  /// Значение колонки assigned_at.
  std::string assignedAt;

  /// Значение колонки unassigned_at.
  odb::nullable<std::string> unassignedAt;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

};

#pragma db object(EmployeePlotAssignmentEntity) table("employee_plot_assignment")
#pragma db member(EmployeePlotAssignmentEntity::id) id auto column("id")
#pragma db member(EmployeePlotAssignmentEntity::farmEmployeeId) column("farm_employee_id")
#pragma db member(EmployeePlotAssignmentEntity::farmPlotId) column("farm_plot_id")
#pragma db member(EmployeePlotAssignmentEntity::assignmentType) column("assignment_type")
#pragma db member(EmployeePlotAssignmentEntity::assignedAt) column("assigned_at")
#pragma db member(EmployeePlotAssignmentEntity::unassignedAt) column("unassigned_at")
#pragma db member(EmployeePlotAssignmentEntity::notes) column("notes")

} // namespace fasc::server::persistence
