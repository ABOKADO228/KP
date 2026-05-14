#pragma once

#include <controllers/dto/Product.hpp>
#include <views/Product.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы product.
inline void to_json(nlohmann::json& json, const ProductEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["type_id"] = value.typeId;
  json["name"] = value.name.null() ? nlohmann::json{nullptr} : nlohmann::json{value.name.get()};
  json["unit_id"] = value.unitId.null() ? nlohmann::json{nullptr} : nlohmann::json{value.unitId.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы product.
inline void from_json(const nlohmann::json& json, ProductCreateDto& value) {
  if (json.contains("type_id") && !json.at("type_id").is_null()) {
    value.typeId = json.at("type_id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("unit_id") && !json.at("unit_id").is_null()) {
    value.unitId = json.at("unit_id").get<int>();
  }
}

/// Читает DTO обновления таблицы product.
inline void from_json(const nlohmann::json& json, ProductUpdateDto& value) {
  if (json.contains("type_id") && !json.at("type_id").is_null()) {
    value.typeId = json.at("type_id").get<int>();
  }
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
  if (json.contains("unit_id") && !json.at("unit_id").is_null()) {
    value.unitId = json.at("unit_id").get<int>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы product.
inline void to_json(nlohmann::json& json, const ProductRowView& view) {
  json = nlohmann::json{{"resource", "product"}, {"data", view.data}};
}

/// Сериализует view списка таблицы product.
inline void to_json(nlohmann::json& json, const ProductRowsView& view) {
  json = nlohmann::json{{"resource", "product"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы product.
inline void to_json(nlohmann::json& json, const ProductMutationView& view) {
  json = nlohmann::json{{"resource", "product"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
