#pragma once

#include <controllers/dto/AssociationRole.hpp>
#include <views/AssociationRole.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы association_role.
inline void to_json(nlohmann::json& json, const AssociationRoleEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["code"] = value.code.null() ? nlohmann::json{nullptr} : nlohmann::json{value.code.get()};
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["description"] = value.description.null() ? nlohmann::json{nullptr} : nlohmann::json{value.description.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы association_role.
inline void from_json(const nlohmann::json& json, AssociationRoleCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

/// Читает DTO обновления таблицы association_role.
inline void from_json(const nlohmann::json& json, AssociationRoleUpdateDto& value) {
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы association_role.
inline void to_json(nlohmann::json& json, const AssociationRoleRowView& view) {
  json = nlohmann::json{{"resource", "association_role"}, {"data", view.data}};
}

/// Сериализует view списка таблицы association_role.
inline void to_json(nlohmann::json& json, const AssociationRoleRowsView& view) {
  json = nlohmann::json{{"resource", "association_role"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы association_role.
inline void to_json(nlohmann::json& json, const AssociationRoleMutationView& view) {
  json = nlohmann::json{{"resource", "association_role"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
