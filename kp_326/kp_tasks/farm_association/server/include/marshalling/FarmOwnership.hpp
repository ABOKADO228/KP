#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/FarmOwnership.hpp>
#include <views/FarmOwnership.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmOwnership.hpp>

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
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("farm_owner_id") && !json.at("farm_owner_id").is_null()) {
    value.farmOwnerId = json.at("farm_owner_id").get<std::uint64_t>();
  }
  if (json.contains("ownership_percentage") && !json.at("ownership_percentage").is_null()) {
    value.ownershipPercentage = json.at("ownership_percentage").get<double>();
  }
  if (json.contains("started_at") && !json.at("started_at").is_null()) {
    value.startedAt = json.at("started_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("ended_at") && !json.at("ended_at").is_null()) {
    value.endedAt = json.at("ended_at").get<fasc::server::domain::Date>();
  }
}

/// Читает DTO обновления таблицы farm_ownership.
inline void from_json(const nlohmann::json& json, FarmOwnershipUpdateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("farm_owner_id") && !json.at("farm_owner_id").is_null()) {
    value.farmOwnerId = json.at("farm_owner_id").get<std::uint64_t>();
  }
  if (json.contains("ownership_percentage") && !json.at("ownership_percentage").is_null()) {
    value.ownershipPercentage = json.at("ownership_percentage").get<double>();
  }
  if (json.contains("started_at") && !json.at("started_at").is_null()) {
    value.startedAt = json.at("started_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("ended_at") && !json.at("ended_at").is_null()) {
    value.endedAt = json.at("ended_at").get<fasc::server::domain::Date>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json FarmOwnershipRowPayload(const FarmOwnershipRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["farm_id"] = view.farmId;
  json["farm_owner_id"] = view.farmOwnerId;
  if (view.ownershipPercentage) {
    json["ownership_percentage"] = *view.ownershipPercentage;
  } else {
    json["ownership_percentage"] = nullptr;
  }
  json["started_at"] = view.startedAt;
  if (view.endedAt) {
    json["ended_at"] = *view.endedAt;
  } else {
    json["ended_at"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmOwnershipRowView toView(const fasc::server::persistence::FarmOwnershipEntity& entity) {
  return FarmOwnershipRowView{
      entity.id,
      entity.farmId,
      entity.farmOwnerId,
      detail::toOptional(entity.ownershipPercentage),
      entity.startedAt,
      detail::toOptional(entity.endedAt)
  };
}

inline FarmOwnershipRowsView toView(const std::vector<fasc::server::persistence::FarmOwnershipEntity>& rows) {
  FarmOwnershipRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmOwnershipRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_ownership";
  json["data"] = detail::FarmOwnershipRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmOwnershipRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_ownership";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmOwnershipRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmOwnershipMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_ownership";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
