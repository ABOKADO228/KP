#pragma once

#include <controllers/dto/Unit.hpp>
#include <views/Unit.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы unit.
inline void to_json(nlohmann::json& json, const UnitEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["code"] = value.code.null() ? nlohmann::json{nullptr} : nlohmann::json{value.code.get()};
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы unit.
inline void from_json(const nlohmann::json& json, UnitCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
}

/// Читает DTO обновления таблицы unit.
inline void from_json(const nlohmann::json& json, UnitUpdateDto& value) {
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы unit.
inline void to_json(nlohmann::json& json, const UnitRowView& view) {
  json = nlohmann::json{{"resource", "unit"}, {"data", view.data}};
}

/// Сериализует view списка таблицы unit.
inline void to_json(nlohmann::json& json, const UnitRowsView& view) {
  json = nlohmann::json{{"resource", "unit"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы unit.
inline void to_json(nlohmann::json& json, const UnitMutationView& view) {
  json = nlohmann::json{{"resource", "unit"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
