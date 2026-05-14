#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/PurchaseOrderItem.hpp>
#include <views/PurchaseOrderItem.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/PurchaseOrderItem.hpp>

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
    value.purchaseOrderId = json.at("purchase_order_id").get<std::uint64_t>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<std::uint64_t>();
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
    value.currency = json.at("currency").get<fasc::server::domain::CurrencyCode>();
  }
}

/// Читает DTO обновления таблицы purchase_order_item.
inline void from_json(const nlohmann::json& json, PurchaseOrderItemUpdateDto& value) {
  if (json.contains("purchase_order_id") && !json.at("purchase_order_id").is_null()) {
    value.purchaseOrderId = json.at("purchase_order_id").get<std::uint64_t>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<std::uint64_t>();
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
    value.currency = json.at("currency").get<fasc::server::domain::CurrencyCode>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json PurchaseOrderItemRowPayload(const PurchaseOrderItemRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["purchase_order_id"] = view.purchaseOrderId;
  json["product_id"] = view.productId;
  if (view.quantity) {
    json["quantity"] = *view.quantity;
  } else {
    json["quantity"] = nullptr;
  }
  if (view.unitPrice) {
    json["unit_price"] = *view.unitPrice;
  } else {
    json["unit_price"] = nullptr;
  }
  if (view.vatRate) {
    json["vat_rate"] = *view.vatRate;
  } else {
    json["vat_rate"] = nullptr;
  }
  if (view.currency) {
    json["currency"] = *view.currency;
  } else {
    json["currency"] = nullptr;
  }
  return json;
}

} // namespace detail

inline PurchaseOrderItemRowView toView(const fasc::server::persistence::PurchaseOrderItemEntity& entity) {
  return PurchaseOrderItemRowView{
      entity.id,
      entity.purchaseOrderId,
      entity.productId,
      detail::toOptional(entity.quantity),
      detail::toOptional(entity.unitPrice),
      detail::toOptional(entity.vatRate),
      detail::toOptional(entity.currency)
  };
}

inline PurchaseOrderItemRowsView toView(const std::vector<fasc::server::persistence::PurchaseOrderItemEntity>& rows) {
  PurchaseOrderItemRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const PurchaseOrderItemRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_order_item";
  json["data"] = detail::PurchaseOrderItemRowPayload(view);
}

inline void to_json(nlohmann::json& json, const PurchaseOrderItemRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_order_item";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::PurchaseOrderItemRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const PurchaseOrderItemMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_order_item";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
