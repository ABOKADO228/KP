#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/PersonDocument.hpp>
#include <views/PersonDocument.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/PersonDocument.hpp>

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
    value.personId = json.at("person_id").get<std::uint64_t>();
  }
  if (json.contains("document_type_id") && !json.at("document_type_id").is_null()) {
    value.documentTypeId = json.at("document_type_id").get<std::uint64_t>();
  }
  if (json.contains("document_number") && !json.at("document_number").is_null()) {
    value.documentNumber = json.at("document_number").get<std::string>();
  }
  if (json.contains("issued_by") && !json.at("issued_by").is_null()) {
    value.issuedBy = json.at("issued_by").get<std::string>();
  }
  if (json.contains("issued_date") && !json.at("issued_date").is_null()) {
    value.issuedDate = json.at("issued_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("expiration_date") && !json.at("expiration_date").is_null()) {
    value.expirationDate = json.at("expiration_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("is_primary") && !json.at("is_primary").is_null()) {
    value.isPrimary = json.at("is_primary").get<bool>();
  }
}

/// Читает DTO обновления таблицы person_document.
inline void from_json(const nlohmann::json& json, PersonDocumentUpdateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<std::uint64_t>();
  }
  if (json.contains("document_type_id") && !json.at("document_type_id").is_null()) {
    value.documentTypeId = json.at("document_type_id").get<std::uint64_t>();
  }
  if (json.contains("document_number") && !json.at("document_number").is_null()) {
    value.documentNumber = json.at("document_number").get<std::string>();
  }
  if (json.contains("issued_by") && !json.at("issued_by").is_null()) {
    value.issuedBy = json.at("issued_by").get<std::string>();
  }
  if (json.contains("issued_date") && !json.at("issued_date").is_null()) {
    value.issuedDate = json.at("issued_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("expiration_date") && !json.at("expiration_date").is_null()) {
    value.expirationDate = json.at("expiration_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("is_primary") && !json.at("is_primary").is_null()) {
    value.isPrimary = json.at("is_primary").get<bool>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json PersonDocumentRowPayload(const PersonDocumentRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["person_id"] = view.personId;
  json["document_type_id"] = view.documentTypeId;
  if (view.documentNumber) {
    json["document_number"] = *view.documentNumber;
  } else {
    json["document_number"] = nullptr;
  }
  if (view.issuedBy) {
    json["issued_by"] = *view.issuedBy;
  } else {
    json["issued_by"] = nullptr;
  }
  if (view.issuedDate) {
    json["issued_date"] = *view.issuedDate;
  } else {
    json["issued_date"] = nullptr;
  }
  if (view.expirationDate) {
    json["expiration_date"] = *view.expirationDate;
  } else {
    json["expiration_date"] = nullptr;
  }
  if (view.isPrimary) {
    json["is_primary"] = *view.isPrimary;
  } else {
    json["is_primary"] = nullptr;
  }
  return json;
}

} // namespace detail

inline PersonDocumentRowView toView(const fasc::server::persistence::PersonDocumentEntity& entity) {
  return PersonDocumentRowView{
      entity.id,
      entity.personId,
      entity.documentTypeId,
      detail::toOptional(entity.documentNumber),
      detail::toOptional(entity.issuedBy),
      detail::toOptional(entity.issuedDate),
      detail::toOptional(entity.expirationDate),
      detail::toOptional(entity.isPrimary)
  };
}

inline PersonDocumentRowsView toView(const std::vector<fasc::server::persistence::PersonDocumentEntity>& rows) {
  PersonDocumentRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const PersonDocumentRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "person_document";
  json["data"] = detail::PersonDocumentRowPayload(view);
}

inline void to_json(nlohmann::json& json, const PersonDocumentRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "person_document";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::PersonDocumentRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const PersonDocumentMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "person_document";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
