#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/FarmRole.hpp>
#include <views/FarmRole.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmRole.hpp>

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
    value.id = json.at("id").get<std::uint64_t>();
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

namespace detail {

inline nlohmann::json FarmRoleRowPayload(const FarmRoleRowView& view) {
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
  return json;
}

} // namespace detail

inline FarmRoleRowView toView(const fasc::server::persistence::FarmRoleEntity& entity) {
  return FarmRoleRowView{
      entity.id,
      detail::toOptional(entity.name),
      detail::toOptional(entity.description)
  };
}

inline FarmRoleRowsView toView(const std::vector<fasc::server::persistence::FarmRoleEntity>& rows) {
  FarmRoleRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmRoleRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_role";
  json["data"] = detail::FarmRoleRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmRoleRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_role";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmRoleRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmRoleMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_role";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
