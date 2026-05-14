#pragma once

#include <controllers/dto/AssociationFarms.hpp>
#include <views/AssociationFarms.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы association_farms.
inline void to_json(nlohmann::json& json, const AssociationFarmsEntity& value) {
  json = nlohmann::json::object();
  json["farm_id"] = value.farmId;
  json["association_id"] = value.associationId;
  json["join_date"] = value.joinDate;
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["notes"] = value.notes.null() ? nlohmann::json{nullptr} : nlohmann::json{value.notes.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы association_farms.
inline void from_json(const nlohmann::json& json, AssociationFarmsCreateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("join_date") && !json.at("join_date").is_null()) {
    value.joinDate = json.at("join_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы association_farms.
inline void from_json(const nlohmann::json& json, AssociationFarmsUpdateDto& value) {
  if (json.contains("join_date") && !json.at("join_date").is_null()) {
    value.joinDate = json.at("join_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы association_farms.
inline void to_json(nlohmann::json& json, const AssociationFarmsRowView& view) {
  json = nlohmann::json{{"resource", "association_farms"}, {"data", view.data}};
}

/// Сериализует view списка таблицы association_farms.
inline void to_json(nlohmann::json& json, const AssociationFarmsRowsView& view) {
  json = nlohmann::json{{"resource", "association_farms"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы association_farms.
inline void to_json(nlohmann::json& json, const AssociationFarmsMutationView& view) {
  json = nlohmann::json{{"resource", "association_farms"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
