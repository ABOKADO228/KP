#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/EmployeePlotAssignment.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы employee_plot_assignment.
struct EmployeePlotAssignmentKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы employee_plot_assignment.
struct EmployeePlotAssignmentCreateDto {
  /// Значение колонки farm_employee_id.
  std::optional<std::uint64_t> farmEmployeeId;

  /// Значение колонки farm_plot_id.
  std::optional<std::uint64_t> farmPlotId;

  /// Значение колонки assignment_type.
  std::optional<fasc::server::domain::EmployeePlotAssignmentType> assignmentType;

  /// Значение колонки assigned_at.
  std::optional<fasc::server::domain::Date> assignedAt;

  /// Значение колонки unassigned_at.
  std::optional<fasc::server::domain::Date> unassignedAt;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы employee_plot_assignment.
struct EmployeePlotAssignmentUpdateDto {
  /// Значение колонки farm_employee_id.
  std::optional<std::uint64_t> farmEmployeeId;

  /// Значение колонки farm_plot_id.
  std::optional<std::uint64_t> farmPlotId;

  /// Значение колонки assignment_type.
  std::optional<fasc::server::domain::EmployeePlotAssignmentType> assignmentType;

  /// Значение колонки assigned_at.
  std::optional<fasc::server::domain::Date> assignedAt;

  /// Значение колонки unassigned_at.
  std::optional<fasc::server::domain::Date> unassignedAt;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO строки таблицы employee_plot_assignment.
struct EmployeePlotAssignmentRowDto {
  /// Данные строки.
  fasc::server::persistence::EmployeePlotAssignmentEntity data;
};

/// DTO списка строк таблицы employee_plot_assignment.
struct EmployeePlotAssignmentRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::EmployeePlotAssignmentEntity> rows;
};

/// DTO результата изменения таблицы employee_plot_assignment.
struct EmployeePlotAssignmentMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
