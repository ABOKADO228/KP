#pragma once

#include <controllers/dto/FarmOwnership.hpp>
#include <views/FarmOwnership.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_ownership.
inline void to_json(nlohmann::json& json, const FarmOwnershipEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["farm_id"] = value.farmId;
  json["farm_owner_id"] = value.farmOwnerId;
  json["ownership_percentage"] = value.ownershipPercentage.null() ? nlohmann::json{nullptr} : nlohmann::json{value.ownershipPercentage.get()};
  json["started_at"] = value.startedAt;
  json["ended_at"] = value.endedAt.null() ? nlohmann::json{nullptr} : nlohmann::json{value.endedAt.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_ownership.
inline void from_json(const nlohmann::json& json, FarmOwnershipCreateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("farm_owner_id") && !json.at("farm_owner_id").is_null()) {
    value.farmOwnerId = json.at("farm_owner_id").get<int>();
  }
  if (json.contains("ownership_percentage") && !json.at("ownership_percentage").is_null()) {
    value.ownershipPercentage = json.at("ownership_percentage").get<double>();
  }
  if (json.contains("started_at") && !json.at("started_at").is_null()) {
    value.startedAt = json.at("started_at").get<std::string>();
  }
  if (json.contains("ended_at") && !json.at("ended_at").is_null()) {
    value.endedAt = json.at("ended_at").get<std::string>();
  }
}

/// Читает DTO обновления таблицы farm_ownership.
inline void from_json(const nlohmann::json& json, FarmOwnershipUpdateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("farm_owner_id") && !json.at("farm_owner_id").is_null()) {
    value.farmOwnerId = json.at("farm_owner_id").get<int>();
  }
  if (json.contains("ownership_percentage") && !json.at("ownership_percentage").is_null()) {
    value.ownershipPercentage = json.at("ownership_percentage").get<double>();
  }
  if (json.contains("started_at") && !json.at("started_at").is_null()) {
    value.startedAt = json.at("started_at").get<std::string>();
  }
  if (json.contains("ended_at") && !json.at("ended_at").is_null()) {
    value.endedAt = json.at("ended_at").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы farm_ownership.
inline void to_json(nlohmann::json& json, const FarmOwnershipRowView& view) {
  json = nlohmann::json{{"resource", "farm_ownership"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_ownership.
inline void to_json(nlohmann::json& json, const FarmOwnershipRowsView& view) {
  json = nlohmann::json{{"resource", "farm_ownership"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_ownership.
inline void to_json(nlohmann::json& json, const FarmOwnershipMutationView& view) {
  json = nlohmann::json{{"resource", "farm_ownership"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
