#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/Supplier.hpp>
#include <views/Supplier.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/Supplier.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы supplier.
inline void to_json(nlohmann::json& json, const SupplierEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["legal_address"] = value.legalAddress.null() ? nlohmann::json{nullptr} : nlohmann::json{value.legalAddress.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы supplier.
inline void from_json(const nlohmann::json& json, SupplierCreateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("legal_address") && !json.at("legal_address").is_null()) {
    value.legalAddress = json.at("legal_address").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::SupplierStatus>();
  }
}

/// Читает DTO обновления таблицы supplier.
inline void from_json(const nlohmann::json& json, SupplierUpdateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("legal_address") && !json.at("legal_address").is_null()) {
    value.legalAddress = json.at("legal_address").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::SupplierStatus>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json SupplierRowPayload(const SupplierRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
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

inline SupplierRowView toView(const fasc::server::persistence::SupplierEntity& entity) {
  return SupplierRowView{
      entity.id,
      detail::toOptional(entity.name),
      detail::toOptional(entity.legalAddress),
      detail::toOptional(entity.status)
  };
}

inline SupplierRowsView toView(const std::vector<fasc::server::persistence::SupplierEntity>& rows) {
  SupplierRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const SupplierRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "supplier";
  json["data"] = detail::SupplierRowPayload(view);
}

inline void to_json(nlohmann::json& json, const SupplierRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "supplier";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::SupplierRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const SupplierMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "supplier";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
