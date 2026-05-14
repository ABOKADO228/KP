#pragma once

#include <controllers/dto/SupplierProductPrice.hpp>
#include <views/SupplierProductPrice.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/SupplierProductPrice.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы supplier_product_price.
inline void to_json(nlohmann::json& json, const SupplierProductPriceEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["supplier_id"] = value.supplierId;
  json["product_id"] = value.productId;
  json["purchase_price"] = value.purchasePrice.null() ? nlohmann::json{nullptr} : nlohmann::json{value.purchasePrice.get()};
  json["valid_from"] = value.validFrom;
  json["valid_until"] = value.validUntil.null() ? nlohmann::json{nullptr} : nlohmann::json{value.validUntil.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы supplier_product_price.
inline void from_json(const nlohmann::json& json, SupplierProductPriceCreateDto& value) {
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("purchase_price") && !json.at("purchase_price").is_null()) {
    value.purchasePrice = json.at("purchase_price").get<double>();
  }
  if (json.contains("valid_from") && !json.at("valid_from").is_null()) {
    value.validFrom = json.at("valid_from").get<std::string>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<std::string>();
  }
}

/// Читает DTO обновления таблицы supplier_product_price.
inline void from_json(const nlohmann::json& json, SupplierProductPriceUpdateDto& value) {
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("purchase_price") && !json.at("purchase_price").is_null()) {
    value.purchasePrice = json.at("purchase_price").get<double>();
  }
  if (json.contains("valid_from") && !json.at("valid_from").is_null()) {
    value.validFrom = json.at("valid_from").get<std::string>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<std::string>();
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

inline nlohmann::json SupplierProductPriceRowPayload(const SupplierProductPriceRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["supplier_id"] = view.supplierId;
  json["product_id"] = view.productId;
  if (view.purchasePrice) {
    json["purchase_price"] = *view.purchasePrice;
  } else {
    json["purchase_price"] = nullptr;
  }
  json["valid_from"] = view.validFrom;
  if (view.validUntil) {
    json["valid_until"] = *view.validUntil;
  } else {
    json["valid_until"] = nullptr;
  }
  return json;
}

} // namespace detail

inline SupplierProductPriceRowView toView(const fasc::server::persistence::SupplierProductPriceEntity& entity) {
  return SupplierProductPriceRowView{
      entity.id,
      entity.supplierId,
      entity.productId,
      detail::toOptional(entity.purchasePrice),
      entity.validFrom,
      detail::toOptional(entity.validUntil)
  };
}

inline SupplierProductPriceRowsView toView(const std::vector<fasc::server::persistence::SupplierProductPriceEntity>& rows) {
  SupplierProductPriceRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const SupplierProductPriceRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "supplier_product_price";
  json["data"] = detail::SupplierProductPriceRowPayload(view);
}

inline void to_json(nlohmann::json& json, const SupplierProductPriceRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "supplier_product_price";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::SupplierProductPriceRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const SupplierProductPriceMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "supplier_product_price";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
