#pragma once

#include <controllers/dto/Farm.hpp>
#include <views/Farm.hpp>

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
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("farm_type") && !json.at("farm_type").is_null()) {
    value.farmType = json.at("farm_type").get<std::string>();
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
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("farm_type") && !json.at("farm_type").is_null()) {
    value.farmType = json.at("farm_type").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы farm.
inline void to_json(nlohmann::json& json, const FarmRowView& view) {
  json = nlohmann::json{{"resource", "farm"}, {"data", view.data}};
}

/// Сериализует view списка таблицы farm.
inline void to_json(nlohmann::json& json, const FarmRowsView& view) {
  json = nlohmann::json{{"resource", "farm"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы farm.
inline void to_json(nlohmann::json& json, const FarmMutationView& view) {
  json = nlohmann::json{{"resource", "farm"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
