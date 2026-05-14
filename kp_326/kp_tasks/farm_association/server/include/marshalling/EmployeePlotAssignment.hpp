#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/EmployeePlotAssignment.hpp>
#include <views/EmployeePlotAssignment.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/EmployeePlotAssignment.hpp>

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
    value.farmEmployeeId = json.at("farm_employee_id").get<std::uint64_t>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<std::uint64_t>();
  }
  if (json.contains("assignment_type") && !json.at("assignment_type").is_null()) {
    value.assignmentType = json.at("assignment_type").get<fasc::server::domain::EmployeePlotAssignmentType>();
  }
  if (json.contains("assigned_at") && !json.at("assigned_at").is_null()) {
    value.assignedAt = json.at("assigned_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("unassigned_at") && !json.at("unassigned_at").is_null()) {
    value.unassignedAt = json.at("unassigned_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы employee_plot_assignment.
inline void from_json(const nlohmann::json& json, EmployeePlotAssignmentUpdateDto& value) {
  if (json.contains("farm_employee_id") && !json.at("farm_employee_id").is_null()) {
    value.farmEmployeeId = json.at("farm_employee_id").get<std::uint64_t>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<std::uint64_t>();
  }
  if (json.contains("assignment_type") && !json.at("assignment_type").is_null()) {
    value.assignmentType = json.at("assignment_type").get<fasc::server::domain::EmployeePlotAssignmentType>();
  }
  if (json.contains("assigned_at") && !json.at("assigned_at").is_null()) {
    value.assignedAt = json.at("assigned_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("unassigned_at") && !json.at("unassigned_at").is_null()) {
    value.unassignedAt = json.at("unassigned_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json EmployeePlotAssignmentRowPayload(const EmployeePlotAssignmentRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["farm_employee_id"] = view.farmEmployeeId;
  json["farm_plot_id"] = view.farmPlotId;
  if (view.assignmentType) {
    json["assignment_type"] = *view.assignmentType;
  } else {
    json["assignment_type"] = nullptr;
  }
  json["assigned_at"] = view.assignedAt;
  if (view.unassignedAt) {
    json["unassigned_at"] = *view.unassignedAt;
  } else {
    json["unassigned_at"] = nullptr;
  }
  if (view.notes) {
    json["notes"] = *view.notes;
  } else {
    json["notes"] = nullptr;
  }
  return json;
}

} // namespace detail

inline EmployeePlotAssignmentRowView toView(const fasc::server::persistence::EmployeePlotAssignmentEntity& entity) {
  return EmployeePlotAssignmentRowView{
      entity.id,
      entity.farmEmployeeId,
      entity.farmPlotId,
      detail::toOptional(entity.assignmentType),
      entity.assignedAt,
      detail::toOptional(entity.unassignedAt),
      detail::toOptional(entity.notes)
  };
}

inline EmployeePlotAssignmentRowsView toView(const std::vector<fasc::server::persistence::EmployeePlotAssignmentEntity>& rows) {
  EmployeePlotAssignmentRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const EmployeePlotAssignmentRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "employee_plot_assignment";
  json["data"] = detail::EmployeePlotAssignmentRowPayload(view);
}

inline void to_json(nlohmann::json& json, const EmployeePlotAssignmentRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "employee_plot_assignment";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::EmployeePlotAssignmentRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const EmployeePlotAssignmentMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "employee_plot_assignment";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
