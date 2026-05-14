#pragma once

#include <controllers/dto/EmploymentStatus.hpp>
#include <views/EmploymentStatus.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы employment_status.
inline void to_json(nlohmann::json& json, const EmploymentStatusEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы employment_status.
inline void from_json(const nlohmann::json& json, EmploymentStatusCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
}

/// Читает DTO обновления таблицы employment_status.
inline void from_json(const nlohmann::json& json, EmploymentStatusUpdateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы employment_status.
inline void to_json(nlohmann::json& json, const EmploymentStatusRowView& view) {
  json = nlohmann::json{{"resource", "employment_status"}, {"data", view.data}};
}

/// Сериализует view списка таблицы employment_status.
inline void to_json(nlohmann::json& json, const EmploymentStatusRowsView& view) {
  json = nlohmann::json{{"resource", "employment_status"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы employment_status.
inline void to_json(nlohmann::json& json, const EmploymentStatusMutationView& view) {
  json = nlohmann::json{{"resource", "employment_status"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
