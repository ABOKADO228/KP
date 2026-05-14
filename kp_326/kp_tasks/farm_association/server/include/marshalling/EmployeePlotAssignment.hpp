#pragma once

#include <controllers/dto/EmployeePlotAssignment.hpp>
#include <views/EmployeePlotAssignment.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы employee_plot_assignment.
inline void to_json(nlohmann::json& json, const EmployeePlotAssignmentEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["farm_employee_id"] = value.farmEmployeeId;
  json["farm_plot_id"] = value.farmPlotId;
  json["assignment_type"] = value.assignmentType.null() ? nlohmann::json{nullptr} : nlohmann::json{value.assignmentType.get()};
  json["assigned_at"] = value.assignedAt;
  json["unassigned_at"] = value.unassignedAt.null() ? nlohmann::json{nullptr} : nlohmann::json{value.unassignedAt.get()};
  json["notes"] = value.notes.null() ? nlohmann::json{nullptr} : nlohmann::json{value.notes.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы employee_plot_assignment.
inline void from_json(const nlohmann::json& json, EmployeePlotAssignmentCreateDto& value) {
  if (json.contains("farm_employee_id") && !json.at("farm_employee_id").is_null()) {
    value.farmEmployeeId = json.at("farm_employee_id").get<int>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<int>();
  }
  if (json.contains("assignment_type") && !json.at("assignment_type").is_null()) {
    value.assignmentType = json.at("assignment_type").get<std::string>();
  }
  if (json.contains("assigned_at") && !json.at("assigned_at").is_null()) {
    value.assignedAt = json.at("assigned_at").get<std::string>();
  }
  if (json.contains("unassigned_at") && !json.at("unassigned_at").is_null()) {
    value.unassignedAt = json.at("unassigned_at").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы employee_plot_assignment.
inline void from_json(const nlohmann::json& json, EmployeePlotAssignmentUpdateDto& value) {
  if (json.contains("farm_employee_id") && !json.at("farm_employee_id").is_null()) {
    value.farmEmployeeId = json.at("farm_employee_id").get<int>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<int>();
  }
  if (json.contains("assignment_type") && !json.at("assignment_type").is_null()) {
    value.assignmentType = json.at("assignment_type").get<std::string>();
  }
  if (json.contains("assigned_at") && !json.at("assigned_at").is_null()) {
    value.assignedAt = json.at("assigned_at").get<std::string>();
  }
  if (json.contains("unassigned_at") && !json.at("unassigned_at").is_null()) {
    value.unassignedAt = json.at("unassigned_at").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы employee_plot_assignment.
inline void to_json(nlohmann::json& json, const EmployeePlotAssignmentRowView& view) {
  json = nlohmann::json{{"resource", "employee_plot_assignment"}, {"data", view.data}};
}

/// Сериализует view списка таблицы employee_plot_assignment.
inline void to_json(nlohmann::json& json, const EmployeePlotAssignmentRowsView& view) {
  json = nlohmann::json{{"resource", "employee_plot_assignment"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы employee_plot_assignment.
inline void to_json(nlohmann::json& json, const EmployeePlotAssignmentMutationView& view) {
  json = nlohmann::json{{"resource", "employee_plot_assignment"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
