#pragma once

#include <controllers/dto/FarmPlotType.hpp>
#include <views/FarmPlotType.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmPlotType.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_plot_type.
inline void to_json(nlohmann::json& json, const FarmPlotTypeEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["description"] = value.description.null() ? nlohmann::json{nullptr} : nlohmann::json{value.description.get()};
  json["farm_plot_level"] = value.farmPlotLevel;
  json["parent_id"] = value.parentId.null() ? nlohmann::json{nullptr} : nlohmann::json{value.parentId.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_plot_type.
inline void from_json(const nlohmann::json& json, FarmPlotTypeCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
  if (json.contains("farm_plot_level") && !json.at("farm_plot_level").is_null()) {
    value.farmPlotLevel = json.at("farm_plot_level").get<int>();
  }
  if (json.contains("parent_id") && !json.at("parent_id").is_null()) {
    value.parentId = json.at("parent_id").get<int>();
  }
}

/// Читает DTO обновления таблицы farm_plot_type.
inline void from_json(const nlohmann::json& json, FarmPlotTypeUpdateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
  if (json.contains("farm_plot_level") && !json.at("farm_plot_level").is_null()) {
    value.farmPlotLevel = json.at("farm_plot_level").get<int>();
  }
  if (json.contains("parent_id") && !json.at("parent_id").is_null()) {
    value.parentId = json.at("parent_id").get<int>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

template <typename T>
inline std::optional<T> toOptional(const odb::nullable<T>& value) {
  if (value.null()) {
    return std::nullopt;
  }
  return value.get();
}

inline nlohmann::json FarmPlotTypeRowPayload(const FarmPlotTypeRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  if (view.description) {
    json["description"] = *view.description;
  } else {
    json["description"] = nullptr;
  }
  json["farm_plot_level"] = view.farmPlotLevel;
  if (view.parentId) {
    json["parent_id"] = *view.parentId;
  } else {
    json["parent_id"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmPlotTypeRowView toView(const fasc::server::persistence::FarmPlotTypeEntity& entity) {
  return FarmPlotTypeRowView{
      entity.id,
      detail::toOptional(entity.name),
      detail::toOptional(entity.description),
      entity.farmPlotLevel,
      detail::toOptional(entity.parentId)
  };
}

inline FarmPlotTypeRowsView toView(const std::vector<fasc::server::persistence::FarmPlotTypeEntity>& rows) {
  FarmPlotTypeRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmPlotTypeRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_type";
  json["data"] = detail::FarmPlotTypeRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmPlotTypeRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_type";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmPlotTypeRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmPlotTypeMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot_type";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
