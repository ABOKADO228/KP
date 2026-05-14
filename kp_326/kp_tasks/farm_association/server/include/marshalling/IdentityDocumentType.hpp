#pragma once

#include <controllers/dto/IdentityDocumentType.hpp>
#include <views/IdentityDocumentType.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/IdentityDocumentType.hpp>

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

namespace detail {

template <typename T>
inline std::optional<T> toOptional(const odb::nullable<T>& value) {
  if (value.null()) {
    return std::nullopt;
  }
  return value.get();
}

inline nlohmann::json IdentityDocumentTypeRowPayload(const IdentityDocumentTypeRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.code) {
    json["code"] = *view.code;
  } else {
    json["code"] = nullptr;
  }
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  if (view.description) {
    json["description"] = *view.description;
  } else {
    json["description"] = nullptr;
  }
  return json;
}

} // namespace detail

inline IdentityDocumentTypeRowView toView(const fasc::server::persistence::IdentityDocumentTypeEntity& entity) {
  return IdentityDocumentTypeRowView{
      entity.id,
      detail::toOptional(entity.code),
      detail::toOptional(entity.name),
      detail::toOptional(entity.description)
  };
}

inline IdentityDocumentTypeRowsView toView(const std::vector<fasc::server::persistence::IdentityDocumentTypeEntity>& rows) {
  IdentityDocumentTypeRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const IdentityDocumentTypeRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "identity_document_type";
  json["data"] = detail::IdentityDocumentTypeRowPayload(view);
}

inline void to_json(nlohmann::json& json, const IdentityDocumentTypeRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "identity_document_type";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::IdentityDocumentTypeRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const IdentityDocumentTypeMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "identity_document_type";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
