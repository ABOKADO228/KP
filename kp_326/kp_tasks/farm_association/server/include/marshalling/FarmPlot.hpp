#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/FarmPlot.hpp>
#include <views/FarmPlot.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmPlot.hpp>

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
    value.farmPlotTypeId = json.at("farm_plot_type_id").get<std::uint64_t>();
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
    value.status = json.at("status").get<fasc::server::domain::FarmPlotStatus>();
  }
}

/// Читает DTO обновления таблицы farm_plot.
inline void from_json(const nlohmann::json& json, FarmPlotUpdateDto& value) {
  if (json.contains("farm_plot_type_id") && !json.at("farm_plot_type_id").is_null()) {
    value.farmPlotTypeId = json.at("farm_plot_type_id").get<std::uint64_t>();
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
    value.status = json.at("status").get<fasc::server::domain::FarmPlotStatus>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json FarmPlotRowPayload(const FarmPlotRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.farmPlotTypeId) {
    json["farm_plot_type_id"] = *view.farmPlotTypeId;
  } else {
    json["farm_plot_type_id"] = nullptr;
  }
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  if (view.area) {
    json["area"] = *view.area;
  } else {
    json["area"] = nullptr;
  }
  if (view.location) {
    json["location"] = *view.location;
  } else {
    json["location"] = nullptr;
  }
  if (view.cadastralNumber) {
    json["cadastral_number"] = *view.cadastralNumber;
  } else {
    json["cadastral_number"] = nullptr;
  }
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmPlotRowView toView(const fasc::server::persistence::FarmPlotEntity& entity) {
  return FarmPlotRowView{
      entity.id,
      detail::toOptional(entity.farmPlotTypeId),
      detail::toOptional(entity.name),
      detail::toOptional(entity.area),
      detail::toOptional(entity.location),
      detail::toOptional(entity.cadastralNumber),
      detail::toOptional(entity.status)
  };
}

inline FarmPlotRowsView toView(const std::vector<fasc::server::persistence::FarmPlotEntity>& rows) {
  FarmPlotRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmPlotRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot";
  json["data"] = detail::FarmPlotRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmPlotRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmPlotRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmPlotMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_plot";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
