#pragma once

#include <controllers/dto/FarmPlotAssignment.hpp>
#include <views/FarmPlotAssignment.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_plot_assignment.
inline void to_json(nlohmann::json& json, const FarmPlotAssignmentEntity& value) {
  json = nlohmann::json::object();
  json["farm_id"] = value.farmId;
  json["farm_plot_id"] = value.farmPlotId;
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["notes"] = value.notes.null() ? nlohmann::json{nullptr} : nlohmann::json{value.notes.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_plot_assignment.
inline void from_json(const nlohmann::json& json, FarmPlotAssignmentCreateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<int>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы farm_plot_assignment.
inline void from_json(const nlohmann::json& json, FarmPlotAssignmentUpdateDto& value) {
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы farm_plot_assignment.
inline void to_json(nlohmann::json& json, const FarmPlotAssignmentRowView& view) {
  json = nlohmann::json{{"resource", "farm_plot_assignment"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_plot_assignment.
inline void to_json(nlohmann::json& json, const FarmPlotAssignmentRowsView& view) {
  json = nlohmann::json{{"resource", "farm_plot_assignment"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_plot_assignment.
inline void to_json(nlohmann::json& json, const FarmPlotAssignmentMutationView& view) {
  json = nlohmann::json{{"resource", "farm_plot_assignment"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
