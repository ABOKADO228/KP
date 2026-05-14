#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/AssociationRole.hpp>
#include <views/AssociationRole.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/AssociationRole.hpp>

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
    value.id = json.at("id").get<std::uint64_t>();
  }
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<fasc::server::domain::AssociationRoleCode>();
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
    value.code = json.at("code").get<fasc::server::domain::AssociationRoleCode>();
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

namespace detail {

inline nlohmann::json AssociationRoleRowPayload(const AssociationRoleRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.code) {
    json["code"] = *view.code;
  } else {
    json["code"] = nullptr;
  }
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
  return json;
}

} // namespace detail

inline AssociationRoleRowView toView(const fasc::server::persistence::AssociationRoleEntity& entity) {
  return AssociationRoleRowView{
      entity.id,
      detail::toOptional(entity.code),
      detail::toOptional(entity.name),
      detail::toOptional(entity.description)
  };
}

inline AssociationRoleRowsView toView(const std::vector<fasc::server::persistence::AssociationRoleEntity>& rows) {
  AssociationRoleRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const AssociationRoleRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_role";
  json["data"] = detail::AssociationRoleRowPayload(view);
}

inline void to_json(nlohmann::json& json, const AssociationRoleRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_role";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::AssociationRoleRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const AssociationRoleMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_role";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
