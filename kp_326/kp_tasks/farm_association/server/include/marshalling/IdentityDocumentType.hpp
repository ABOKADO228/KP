#pragma once

#include <controllers/dto/IdentityDocumentType.hpp>
#include <views/IdentityDocumentType.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы identity_document_type.
inline void to_json(nlohmann::json& json, const IdentityDocumentTypeEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["code"] = value.code.null() ? nlohmann::json{nullptr} : nlohmann::json{value.code.get()};
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["description"] = value.description.null() ? nlohmann::json{nullptr} : nlohmann::json{value.description.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы identity_document_type.
inline void from_json(const nlohmann::json& json, IdentityDocumentTypeCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

/// Читает DTO обновления таблицы identity_document_type.
inline void from_json(const nlohmann::json& json, IdentityDocumentTypeUpdateDto& value) {
  if (json.contains("code") && !json.at("code").is_null()) {
    value.code = json.at("code").get<std::string>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы identity_document_type.
inline void to_json(nlohmann::json& json, const IdentityDocumentTypeRowView& view) {
  json = nlohmann::json{{"resource", "identity_document_type"}, {"data", view.data}};
}

/// Сериализует view списка таблицы identity_document_type.
inline void to_json(nlohmann::json& json, const IdentityDocumentTypeRowsView& view) {
  json = nlohmann::json{{"resource", "identity_document_type"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы identity_document_type.
inline void to_json(nlohmann::json& json, const IdentityDocumentTypeMutationView& view) {
  json = nlohmann::json{{"resource", "identity_document_type"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
