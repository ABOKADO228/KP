#pragma once

#include <controllers/dto/PersonDocument.hpp>
#include <views/PersonDocument.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы person_document.
inline void to_json(nlohmann::json& json, const PersonDocumentEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["person_id"] = value.personId;
  json["document_type_id"] = value.documentTypeId;
  json["document_number"] = value.documentNumber.null() ? nlohmann::json{nullptr} : nlohmann::json{value.documentNumber.get()};
  json["issued_by"] = value.issuedBy.null() ? nlohmann::json{nullptr} : nlohmann::json{value.issuedBy.get()};
  json["issued_date"] = value.issuedDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.issuedDate.get()};
  json["expiration_date"] = value.expirationDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.expirationDate.get()};
  json["is_primary"] = value.isPrimary.null() ? nlohmann::json{nullptr} : nlohmann::json{value.isPrimary.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы person_document.
inline void from_json(const nlohmann::json& json, PersonDocumentCreateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("document_type_id") && !json.at("document_type_id").is_null()) {
    value.documentTypeId = json.at("document_type_id").get<int>();
  }
  if (json.contains("document_number") && !json.at("document_number").is_null()) {
    value.documentNumber = json.at("document_number").get<std::string>();
  }
  if (json.contains("issued_by") && !json.at("issued_by").is_null()) {
    value.issuedBy = json.at("issued_by").get<std::string>();
  }
  if (json.contains("issued_date") && !json.at("issued_date").is_null()) {
    value.issuedDate = json.at("issued_date").get<std::string>();
  }
  if (json.contains("expiration_date") && !json.at("expiration_date").is_null()) {
    value.expirationDate = json.at("expiration_date").get<std::string>();
  }
  if (json.contains("is_primary") && !json.at("is_primary").is_null()) {
    value.isPrimary = json.at("is_primary").get<int>();
  }
}

/// Читает DTO обновления таблицы person_document.
inline void from_json(const nlohmann::json& json, PersonDocumentUpdateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("document_type_id") && !json.at("document_type_id").is_null()) {
    value.documentTypeId = json.at("document_type_id").get<int>();
  }
  if (json.contains("document_number") && !json.at("document_number").is_null()) {
    value.documentNumber = json.at("document_number").get<std::string>();
  }
  if (json.contains("issued_by") && !json.at("issued_by").is_null()) {
    value.issuedBy = json.at("issued_by").get<std::string>();
  }
  if (json.contains("issued_date") && !json.at("issued_date").is_null()) {
    value.issuedDate = json.at("issued_date").get<std::string>();
  }
  if (json.contains("expiration_date") && !json.at("expiration_date").is_null()) {
    value.expirationDate = json.at("expiration_date").get<std::string>();
  }
  if (json.contains("is_primary") && !json.at("is_primary").is_null()) {
    value.isPrimary = json.at("is_primary").get<int>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы person_document.
inline void to_json(nlohmann::json& json, const PersonDocumentRowView& view) {
  json = nlohmann::json{{"resource", "person_document"}, {"data", view.data}};
}

/// Сериализует view списка таблицы person_document.
inline void to_json(nlohmann::json& json, const PersonDocumentRowsView& view) {
  json = nlohmann::json{{"resource", "person_document"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы person_document.
inline void to_json(nlohmann::json& json, const PersonDocumentMutationView& view) {
  json = nlohmann::json{{"resource", "person_document"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
