#pragma once

#include <controllers/dto/Supplier.hpp>
#include <views/Supplier.hpp>

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
    value.status = json.at("status").get<std::string>();
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
    value.status = json.at("status").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы supplier.
inline void to_json(nlohmann::json& json, const SupplierRowView& view) {
  json = nlohmann::json{{"resource", "supplier"}, {"data", view.data}};
}

/// Сериализует view списка таблицы supplier.
inline void to_json(nlohmann::json& json, const SupplierRowsView& view) {
  json = nlohmann::json{{"resource", "supplier"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы supplier.
inline void to_json(nlohmann::json& json, const SupplierMutationView& view) {
  json = nlohmann::json{{"resource", "supplier"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
