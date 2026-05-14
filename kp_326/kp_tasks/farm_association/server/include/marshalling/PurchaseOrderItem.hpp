#pragma once

#include <controllers/dto/PurchaseOrderItem.hpp>
#include <views/PurchaseOrderItem.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы purchase_order_item.
inline void to_json(nlohmann::json& json, const PurchaseOrderItemEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["purchase_order_id"] = value.purchaseOrderId;
  json["product_id"] = value.productId;
  json["quantity"] = value.quantity.null() ? nlohmann::json{nullptr} : nlohmann::json{value.quantity.get()};
  json["unit_price"] = value.unitPrice.null() ? nlohmann::json{nullptr} : nlohmann::json{value.unitPrice.get()};
  json["vat_rate"] = value.vatRate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.vatRate.get()};
  json["currency"] = value.currency.null() ? nlohmann::json{nullptr} : nlohmann::json{value.currency.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы purchase_order_item.
inline void from_json(const nlohmann::json& json, PurchaseOrderItemCreateDto& value) {
  if (json.contains("purchase_order_id") && !json.at("purchase_order_id").is_null()) {
    value.purchaseOrderId = json.at("purchase_order_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<double>();
  }
  if (json.contains("unit_price") && !json.at("unit_price").is_null()) {
    value.unitPrice = json.at("unit_price").get<double>();
  }
  if (json.contains("vat_rate") && !json.at("vat_rate").is_null()) {
    value.vatRate = json.at("vat_rate").get<double>();
  }
  if (json.contains("currency") && !json.at("currency").is_null()) {
    value.currency = json.at("currency").get<std::string>();
  }
}

/// Читает DTO обновления таблицы purchase_order_item.
inline void from_json(const nlohmann::json& json, PurchaseOrderItemUpdateDto& value) {
  if (json.contains("purchase_order_id") && !json.at("purchase_order_id").is_null()) {
    value.purchaseOrderId = json.at("purchase_order_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<double>();
  }
  if (json.contains("unit_price") && !json.at("unit_price").is_null()) {
    value.unitPrice = json.at("unit_price").get<double>();
  }
  if (json.contains("vat_rate") && !json.at("vat_rate").is_null()) {
    value.vatRate = json.at("vat_rate").get<double>();
  }
  if (json.contains("currency") && !json.at("currency").is_null()) {
    value.currency = json.at("currency").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы purchase_order_item.
inline void to_json(nlohmann::json& json, const PurchaseOrderItemRowView& view) {
  json = nlohmann::json{{"resource", "purchase_order_item"}, {"data", view.data}};
}

/// Сериализует view списка таблицы purchase_order_item.
inline void to_json(nlohmann::json& json, const PurchaseOrderItemRowsView& view) {
  json = nlohmann::json{{"resource", "purchase_order_item"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы purchase_order_item.
inline void to_json(nlohmann::json& json, const PurchaseOrderItemMutationView& view) {
  json = nlohmann::json{{"resource", "purchase_order_item"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
