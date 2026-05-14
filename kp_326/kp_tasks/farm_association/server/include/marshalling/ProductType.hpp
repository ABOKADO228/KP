#pragma once

#include <controllers/dto/ProductType.hpp>
#include <views/ProductType.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы product_type.
inline void to_json(nlohmann::json& json, const ProductTypeEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["parent_id"] = value.parentId.null() ? nlohmann::json{nullptr} : nlohmann::json{value.parentId.get()};
  json["sku"] = value.sku.null() ? nlohmann::json{nullptr} : nlohmann::json{value.sku.get()};
  json["product_level"] = value.productLevel;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["description"] = value.description.null() ? nlohmann::json{nullptr} : nlohmann::json{value.description.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы product_type.
inline void from_json(const nlohmann::json& json, ProductTypeCreateDto& value) {
  if (json.contains("id") && !json.at("id").is_null()) {
    value.id = json.at("id").get<int>();
  }
  if (json.contains("parent_id") && !json.at("parent_id").is_null()) {
    value.parentId = json.at("parent_id").get<int>();
  }
  if (json.contains("sku") && !json.at("sku").is_null()) {
    value.sku = json.at("sku").get<std::string>();
  }
  if (json.contains("product_level") && !json.at("product_level").is_null()) {
    value.productLevel = json.at("product_level").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

/// Читает DTO обновления таблицы product_type.
inline void from_json(const nlohmann::json& json, ProductTypeUpdateDto& value) {
  if (json.contains("parent_id") && !json.at("parent_id").is_null()) {
    value.parentId = json.at("parent_id").get<int>();
  }
  if (json.contains("sku") && !json.at("sku").is_null()) {
    value.sku = json.at("sku").get<std::string>();
  }
  if (json.contains("product_level") && !json.at("product_level").is_null()) {
    value.productLevel = json.at("product_level").get<int>();
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

/// Сериализует view строки таблицы product_type.
inline void to_json(nlohmann::json& json, const ProductTypeRowView& view) {
  json = nlohmann::json{{"resource", "product_type"}, {"data", view.data}};
}

/// Сериализует view списка таблицы product_type.
inline void to_json(nlohmann::json& json, const ProductTypeRowsView& view) {
  json = nlohmann::json{{"resource", "product_type"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы product_type.
inline void to_json(nlohmann::json& json, const ProductTypeMutationView& view) {
  json = nlohmann::json{{"resource", "product_type"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
