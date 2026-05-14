#pragma once

#include <controllers/dto/FarmOwner.hpp>
#include <views/FarmOwner.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_owner.
inline void to_json(nlohmann::json& json, const FarmOwnerEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["person_id"] = value.personId;
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["rating"] = value.rating.null() ? nlohmann::json{nullptr} : nlohmann::json{value.rating.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_owner.
inline void from_json(const nlohmann::json& json, FarmOwnerCreateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("rating") && !json.at("rating").is_null()) {
    value.rating = json.at("rating").get<double>();
  }
}

/// Читает DTO обновления таблицы farm_owner.
inline void from_json(const nlohmann::json& json, FarmOwnerUpdateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("rating") && !json.at("rating").is_null()) {
    value.rating = json.at("rating").get<double>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы farm_owner.
inline void to_json(nlohmann::json& json, const FarmOwnerRowView& view) {
  json = nlohmann::json{{"resource", "farm_owner"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_owner.
inline void to_json(nlohmann::json& json, const FarmOwnerRowsView& view) {
  json = nlohmann::json{{"resource", "farm_owner"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_owner.
inline void to_json(nlohmann::json& json, const FarmOwnerMutationView& view) {
  json = nlohmann::json{{"resource", "farm_owner"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
