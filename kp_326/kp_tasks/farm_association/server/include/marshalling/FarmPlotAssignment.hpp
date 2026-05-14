#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/FarmPlotAssignment.hpp>
#include <views/FarmPlotAssignment.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmPlotAssignment.hpp>

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
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("farm_plot_id") && !json.at("farm_plot_id").is_null()) {
    value.farmPlotId = json.at("farm_plot_id").get<std::uint64_t>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::FarmPlotAssignmentStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы farm_plot_assignment.
inline void from_json(const nlohmann::json& json, FarmPlotAssignmentUpdateDto& value) {
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::FarmPlotAssignmentStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json FarmPlotAssignmentRowPayload(const FarmPlotAssignmentRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["farm_id"] = view.farmId;
  json["farm_plot_id"] = view.farmPlotId;
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.notes) {
    json["notes"] = *view.notes;
  } else {
    json["notes"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmPlotAssignmentRowView toView(const fasc::server::persistence::FarmPlotAssignmentEntity& entity) {
  return FarmPlotAssignmentRowView{
      entity.farmId,
      entity.farmPlotId,
      detail::toOptional(entity.status),
      detail::toOptional(entity.notes)
  };
}

inline FarmPlotAssignmentRowsView toView(const std::vector<fasc::server::persistence::FarmPlotAssignmentEntity>& rows) {
  FarmPlotAssignmentRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmPlotAssignmentRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_assignment";
  json["data"] = detail::FarmPlotAssignmentRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmPlotAssignmentRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_assignment";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmPlotAssignmentRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmPlotAssignmentMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_assignment";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
