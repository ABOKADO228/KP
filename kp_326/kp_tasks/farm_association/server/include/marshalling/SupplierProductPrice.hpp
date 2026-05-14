#pragma once

#include <controllers/dto/SupplierProductPrice.hpp>
#include <views/SupplierProductPrice.hpp>

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

/// Сериализует view строки таблицы supplier_product_price.
inline void to_json(nlohmann::json& json, const SupplierProductPriceRowView& view) {
  json = nlohmann::json{{"resource", "supplier_product_price"}, {"data", view.data}};
}

/// Сериализует view списка таблицы supplier_product_price.
inline void to_json(nlohmann::json& json, const SupplierProductPriceRowsView& view) {
  json = nlohmann::json{{"resource", "supplier_product_price"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы supplier_product_price.
inline void to_json(nlohmann::json& json, const SupplierProductPriceMutationView& view) {
  json = nlohmann::json{{"resource", "supplier_product_price"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
