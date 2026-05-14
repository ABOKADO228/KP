#pragma once

#include <controllers/dto/FarmPlot.hpp>
#include <views/FarmPlot.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_plot.
inline void to_json(nlohmann::json& json, const FarmPlotEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["farm_plot_type_id"] = value.farmPlotTypeId.null() ? nlohmann::json{nullptr} : nlohmann::json{value.farmPlotTypeId.get()};
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["area"] = value.area.null() ? nlohmann::json{nullptr} : nlohmann::json{value.area.get()};
  json["location"] = value.location.null() ? nlohmann::json{nullptr} : nlohmann::json{value.location.get()};
  json["cadastral_number"] = value.cadastralNumber.null() ? nlohmann::json{nullptr} : nlohmann::json{value.cadastralNumber.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_plot.
inline void from_json(const nlohmann::json& json, FarmPlotCreateDto& value) {
  if (json.contains("farm_plot_type_id") && !json.at("farm_plot_type_id").is_null()) {
    value.farmPlotTypeId = json.at("farm_plot_type_id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("area") && !json.at("area").is_null()) {
    value.area = json.at("area").get<double>();
  }
  if (json.contains("location") && !json.at("location").is_null()) {
    value.location = json.at("location").get<std::string>();
  }
  if (json.contains("cadastral_number") && !json.at("cadastral_number").is_null()) {
    value.cadastralNumber = json.at("cadastral_number").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
}

/// Читает DTO обновления таблицы farm_plot.
inline void from_json(const nlohmann::json& json, FarmPlotUpdateDto& value) {
  if (json.contains("farm_plot_type_id") && !json.at("farm_plot_type_id").is_null()) {
    value.farmPlotTypeId = json.at("farm_plot_type_id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("area") && !json.at("area").is_null()) {
    value.area = json.at("area").get<double>();
  }
  if (json.contains("location") && !json.at("location").is_null()) {
    value.location = json.at("location").get<std::string>();
  }
  if (json.contains("cadastral_number") && !json.at("cadastral_number").is_null()) {
    value.cadastralNumber = json.at("cadastral_number").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы farm_plot.
inline void to_json(nlohmann::json& json, const FarmPlotRowView& view) {
  json = nlohmann::json{{"resource", "farm_plot"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_plot.
inline void to_json(nlohmann::json& json, const FarmPlotRowsView& view) {
  json = nlohmann::json{{"resource", "farm_plot"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_plot.
inline void to_json(nlohmann::json& json, const FarmPlotMutationView& view) {
  json = nlohmann::json{{"resource", "farm_plot"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
