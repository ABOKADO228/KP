#pragma once

#include <controllers/dto/ProductType.hpp>
#include <views/ProductType.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/ProductType.hpp>

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

namespace detail {

template <typename T>
inline std::optional<T> toOptional(const odb::nullable<T>& value) {
  if (value.null()) {
    return std::nullopt;
  }
  return value.get();
}

inline nlohmann::json ProductTypeRowPayload(const ProductTypeRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.parentId) {
    json["parent_id"] = *view.parentId;
  } else {
    json["parent_id"] = nullptr;
  }
  if (view.sku) {
    json["sku"] = *view.sku;
  } else {
    json["sku"] = nullptr;
  }
  json["product_level"] = view.productLevel;
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

inline ProductTypeRowView toView(const fasc::server::persistence::ProductTypeEntity& entity) {
  return ProductTypeRowView{
      entity.id,
      detail::toOptional(entity.parentId),
      detail::toOptional(entity.sku),
      entity.productLevel,
      detail::toOptional(entity.name),
      detail::toOptional(entity.description)
  };
}

inline ProductTypeRowsView toView(const std::vector<fasc::server::persistence::ProductTypeEntity>& rows) {
  ProductTypeRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const ProductTypeRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "product_type";
  json["data"] = detail::ProductTypeRowPayload(view);
}

inline void to_json(nlohmann::json& json, const ProductTypeRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "product_type";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::ProductTypeRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const ProductTypeMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "product_type";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
