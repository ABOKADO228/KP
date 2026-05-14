#pragma once

#include <controllers/dto/FarmRole.hpp>
#include <views/FarmRole.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_role.
inline void to_json(nlohmann::json& json, const FarmRoleEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["description"] = value.description.null() ? nlohmann::json{nullptr} : nlohmann::json{value.description.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_role.
inline void from_json(const nlohmann::json& json, FarmRoleCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

/// Читает DTO обновления таблицы farm_role.
inline void from_json(const nlohmann::json& json, FarmRoleUpdateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы farm_role.
inline void to_json(nlohmann::json& json, const FarmRoleRowView& view) {
  json = nlohmann::json{{"resource", "farm_role"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_role.
inline void to_json(nlohmann::json& json, const FarmRoleRowsView& view) {
  json = nlohmann::json{{"resource", "farm_role"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_role.
inline void to_json(nlohmann::json& json, const FarmRoleMutationView& view) {
  json = nlohmann::json{{"resource", "farm_role"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
