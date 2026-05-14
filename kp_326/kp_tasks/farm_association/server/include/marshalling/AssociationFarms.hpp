#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/AssociationFarms.hpp>
#include <views/AssociationFarms.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/AssociationFarms.hpp>

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
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<std::uint64_t>();
  }
  if (json.contains("join_date") && !json.at("join_date").is_null()) {
    value.joinDate = json.at("join_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::AssociationFarmsStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы association_farms.
inline void from_json(const nlohmann::json& json, AssociationFarmsUpdateDto& value) {
  if (json.contains("join_date") && !json.at("join_date").is_null()) {
    value.joinDate = json.at("join_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::AssociationFarmsStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json AssociationFarmsRowPayload(const AssociationFarmsRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["farm_id"] = view.farmId;
  json["association_id"] = view.associationId;
  json["join_date"] = view.joinDate;
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.notes) {
    json["notes"] = *view.notes;
  } else {
    json["notes"] = nullptr;
  }
  return json;
}

} // namespace detail

inline AssociationFarmsRowView toView(const fasc::server::persistence::AssociationFarmsEntity& entity) {
  return AssociationFarmsRowView{
      entity.farmId,
      entity.associationId,
      entity.joinDate,
      detail::toOptional(entity.status),
      detail::toOptional(entity.notes)
  };
}

inline AssociationFarmsRowsView toView(const std::vector<fasc::server::persistence::AssociationFarmsEntity>& rows) {
  AssociationFarmsRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const AssociationFarmsRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_farms";
  json["data"] = detail::AssociationFarmsRowPayload(view);
}

inline void to_json(nlohmann::json& json, const AssociationFarmsRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_farms";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::AssociationFarmsRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const AssociationFarmsMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_farms";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
