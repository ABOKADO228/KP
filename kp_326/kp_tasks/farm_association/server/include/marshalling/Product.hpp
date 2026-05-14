#pragma once

#include <controllers/dto/Product.hpp>
#include <views/Product.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/Product.hpp>

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

namespace detail {

template <typename T>
inline std::optional<T> toOptional(const odb::nullable<T>& value) {
  if (value.null()) {
    return std::nullopt;
  }
  return value.get();
}

inline nlohmann::json ProductRowPayload(const ProductRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["type_id"] = view.typeId;
  if (view.name) {
    json["name"] = *view.name;
  } else {
    json["name"] = nullptr;
  }
  if (view.unitId) {
    json["unit_id"] = *view.unitId;
  } else {
    json["unit_id"] = nullptr;
  }
  return json;
}

} // namespace detail

inline ProductRowView toView(const fasc::server::persistence::ProductEntity& entity) {
  return ProductRowView{
      entity.id,
      entity.typeId,
      detail::toOptional(entity.name),
      detail::toOptional(entity.unitId)
  };
}

inline ProductRowsView toView(const std::vector<fasc::server::persistence::ProductEntity>& rows) {
  ProductRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const ProductRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "product";
  json["data"] = detail::ProductRowPayload(view);
}

inline void to_json(nlohmann::json& json, const ProductRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "product";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::ProductRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const ProductMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "product";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
