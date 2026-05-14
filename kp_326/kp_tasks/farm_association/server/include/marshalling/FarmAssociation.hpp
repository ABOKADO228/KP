#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/FarmAssociation.hpp>
#include <views/FarmAssociation.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmAssociation.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_association.
inline void to_json(nlohmann::json& json, const FarmAssociationEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["inn"] = value.inn.null() ? nlohmann::json{nullptr} : nlohmann::json{value.inn.get()};
  json["ogrn"] = value.ogrn.null() ? nlohmann::json{nullptr} : nlohmann::json{value.ogrn.get()};
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["legal_address"] = value.legalAddress.null() ? nlohmann::json{nullptr} : nlohmann::json{value.legalAddress.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_association.
inline void from_json(const nlohmann::json& json, FarmAssociationCreateDto& value) {
  if (json.contains("inn") && !json.at("inn").is_null()) {
    value.inn = json.at("inn").get<std::string>();
  }
  if (json.contains("ogrn") && !json.at("ogrn").is_null()) {
    value.ogrn = json.at("ogrn").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("legal_address") && !json.at("legal_address").is_null()) {
    value.legalAddress = json.at("legal_address").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::FarmAssociationStatus>();
  }
}

/// Читает DTO обновления таблицы farm_association.
inline void from_json(const nlohmann::json& json, FarmAssociationUpdateDto& value) {
  if (json.contains("inn") && !json.at("inn").is_null()) {
    value.inn = json.at("inn").get<std::string>();
  }
  if (json.contains("ogrn") && !json.at("ogrn").is_null()) {
    value.ogrn = json.at("ogrn").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("legal_address") && !json.at("legal_address").is_null()) {
    value.legalAddress = json.at("legal_address").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::FarmAssociationStatus>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json FarmAssociationRowPayload(const FarmAssociationRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
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
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  if (view.legalAddress) {
    json["legal_address"] = *view.legalAddress;
  } else {
    json["legal_address"] = nullptr;
  }
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmAssociationRowView toView(const fasc::server::persistence::FarmAssociationEntity& entity) {
  return FarmAssociationRowView{
      entity.id,
      detail::toOptional(entity.inn),
      detail::toOptional(entity.ogrn),
      detail::toOptional(entity.name),
      detail::toOptional(entity.legalAddress),
      detail::toOptional(entity.status)
  };
}

inline FarmAssociationRowsView toView(const std::vector<fasc::server::persistence::FarmAssociationEntity>& rows) {
  FarmAssociationRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmAssociationRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_association";
  json["data"] = detail::FarmAssociationRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmAssociationRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_association";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmAssociationRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmAssociationMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_association";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
