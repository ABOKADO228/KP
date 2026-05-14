#pragma once

#include <controllers/dto/FarmAssociation.hpp>
#include <views/FarmAssociation.hpp>

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
    value.status = json.at("status").get<std::string>();
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
    value.status = json.at("status").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы farm_association.
inline void to_json(nlohmann::json& json, const FarmAssociationRowView& view) {
  json = nlohmann::json{{"resource", "farm_association"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm_association.
inline void to_json(nlohmann::json& json, const FarmAssociationRowsView& view) {
  json = nlohmann::json{{"resource", "farm_association"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm_association.
inline void to_json(nlohmann::json& json, const FarmAssociationMutationView& view) {
  json = nlohmann::json{{"resource", "farm_association"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
