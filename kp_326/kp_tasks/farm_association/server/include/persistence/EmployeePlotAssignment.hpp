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

/// Сущность таблицы employee_plot_assignment.
struct EmployeePlotAssignmentEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки farm_employee_id.
  std::uint64_t farmEmployeeId{};

  /// Значение колонки farm_plot_id.
  std::uint64_t farmPlotId{};

  /// Значение колонки assignment_type.
  odb::nullable<fasc::server::domain::EmployeePlotAssignmentType> assignmentType;

  /// Значение колонки assigned_at.
  fasc::server::domain::Date assignedAt;

  /// Значение колонки unassigned_at.
  odb::nullable<fasc::server::domain::Date> unassignedAt;

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

} // namespace persistence
} // namespace server
} // namespace fasc
