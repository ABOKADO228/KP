#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/Farm.hpp>
#include <views/Farm.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/Farm.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm.
inline void to_json(nlohmann::json& json, const FarmEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["legal_name"] = value.legalName.null() ? nlohmann::json{nullptr} : nlohmann::json{value.legalName.get()};
  json["legal_address"] = value.legalAddress.null() ? nlohmann::json{nullptr} : nlohmann::json{value.legalAddress.get()};
  json["actual_address"] = value.actualAddress.null() ? nlohmann::json{nullptr} : nlohmann::json{value.actualAddress.get()};
  json["inn"] = value.inn.null() ? nlohmann::json{nullptr} : nlohmann::json{value.inn.get()};
  json["ogrn"] = value.ogrn.null() ? nlohmann::json{nullptr} : nlohmann::json{value.ogrn.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["farm_type"] = value.farmType.null() ? nlohmann::json{nullptr} : nlohmann::json{value.farmType.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm.
inline void from_json(const nlohmann::json& json, FarmCreateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("legal_name") && !json.at("legal_name").is_null()) {
    value.legalName = json.at("legal_name").get<std::string>();
  }
  if (json.contains("legal_address") && !json.at("legal_address").is_null()) {
    value.legalAddress = json.at("legal_address").get<std::string>();
  }
  if (json.contains("actual_address") && !json.at("actual_address").is_null()) {
    value.actualAddress = json.at("actual_address").get<std::string>();
  }
  if (json.contains("inn") && !json.at("inn").is_null()) {
    value.inn = json.at("inn").get<std::string>();
  }
  if (json.contains("ogrn") && !json.at("ogrn").is_null()) {
    value.ogrn = json.at("ogrn").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::FarmStatus>();
  }
  if (json.contains("farm_type") && !json.at("farm_type").is_null()) {
    value.farmType = json.at("farm_type").get<fasc::server::domain::FarmType>();
  }
}

/// Читает DTO обновления таблицы farm.
inline void from_json(const nlohmann::json& json, FarmUpdateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("legal_name") && !json.at("legal_name").is_null()) {
    value.legalName = json.at("legal_name").get<std::string>();
  }
  if (json.contains("legal_address") && !json.at("legal_address").is_null()) {
    value.legalAddress = json.at("legal_address").get<std::string>();
  }
  if (json.contains("actual_address") && !json.at("actual_address").is_null()) {
    value.actualAddress = json.at("actual_address").get<std::string>();
  }
  if (json.contains("inn") && !json.at("inn").is_null()) {
    value.inn = json.at("inn").get<std::string>();
  }
  if (json.contains("ogrn") && !json.at("ogrn").is_null()) {
    value.ogrn = json.at("ogrn").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::FarmStatus>();
  }
  if (json.contains("farm_type") && !json.at("farm_type").is_null()) {
    value.farmType = json.at("farm_type").get<fasc::server::domain::FarmType>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json FarmRowPayload(const FarmRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  if (view.legalName) {
    json["legal_name"] = *view.legalName;
  } else {
    json["legal_name"] = nullptr;
  }
  if (view.legalAddress) {
    json["legal_address"] = *view.legalAddress;
  } else {
    json["legal_address"] = nullptr;
  }
  if (view.actualAddress) {
    json["actual_address"] = *view.actualAddress;
  } else {
    json["actual_address"] = nullptr;
  }
  if (view.inn) {
    json["inn"] = *view.inn;
  } else {
    json["inn"] = nullptr;
  }
  if (view.ogrn) {
    json["ogrn"] = *view.ogrn;
  } else {
    json["ogrn"] = nullptr;
  }
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.farmType) {
    json["farm_type"] = *view.farmType;
  } else {
    json["farm_type"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmRowView toView(const fasc::server::persistence::FarmEntity& entity) {
  return FarmRowView{
      entity.id,
      detail::toOptional(entity.name),
      detail::toOptional(entity.legalName),
      detail::toOptional(entity.legalAddress),
      detail::toOptional(entity.actualAddress),
      detail::toOptional(entity.inn),
      detail::toOptional(entity.ogrn),
      detail::toOptional(entity.status),
      detail::toOptional(entity.farmType)
  };
}

inline FarmRowsView toView(const std::vector<fasc::server::persistence::FarmEntity>& rows) {
  FarmRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm";
  json["data"] = detail::FarmRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
