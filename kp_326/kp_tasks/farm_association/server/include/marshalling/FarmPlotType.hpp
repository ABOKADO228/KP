#pragma once

#include <controllers/dto/FarmPlotType.hpp>
#include <views/FarmPlotType.hpp>

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

/// Сериализует view строки таблицы farm_plot_type.
inline void to_json(nlohmann::json& json, const FarmPlotTypeRowView& view) {
  json = nlohmann::json{{"resource", "farm_plot_type"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_plot_type.
inline void to_json(nlohmann::json& json, const FarmPlotTypeRowsView& view) {
  json = nlohmann::json{{"resource", "farm_plot_type"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_plot_type.
inline void to_json(nlohmann::json& json, const FarmPlotTypeMutationView& view) {
  json = nlohmann::json{{"resource", "farm_plot_type"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
