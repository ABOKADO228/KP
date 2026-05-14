#pragma once

#include <controllers/dto/PurchaseOrder.hpp>
#include <views/PurchaseOrder.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы purchase_order.
inline void to_json(nlohmann::json& json, const PurchaseOrderEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["association_id"] = value.associationId;
  json["supplier_id"] = value.supplierId;
  json["delivery_address"] = value.deliveryAddress.null() ? nlohmann::json{nullptr} : nlohmann::json{value.deliveryAddress.get()};
  json["order_date"] = value.orderDate;
  json["expected_delivery_date"] = value.expectedDeliveryDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.expectedDeliveryDate.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["total_amount"] = value.totalAmount.null() ? nlohmann::json{nullptr} : nlohmann::json{value.totalAmount.get()};
  json["received_at"] = value.receivedAt.null() ? nlohmann::json{nullptr} : nlohmann::json{value.receivedAt.get()};
  json["created_by"] = value.createdBy.null() ? nlohmann::json{nullptr} : nlohmann::json{value.createdBy.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы purchase_order.
inline void from_json(const nlohmann::json& json, PurchaseOrderCreateDto& value) {
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<int>();
  }
  if (json.contains("delivery_address") && !json.at("delivery_address").is_null()) {
    value.deliveryAddress = json.at("delivery_address").get<std::string>();
  }
  if (json.contains("order_date") && !json.at("order_date").is_null()) {
    value.orderDate = json.at("order_date").get<std::string>();
  }
  if (json.contains("expected_delivery_date") && !json.at("expected_delivery_date").is_null()) {
    value.expectedDeliveryDate = json.at("expected_delivery_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("total_amount") && !json.at("total_amount").is_null()) {
    value.totalAmount = json.at("total_amount").get<double>();
  }
  if (json.contains("received_at") && !json.at("received_at").is_null()) {
    value.receivedAt = json.at("received_at").get<std::string>();
  }
  if (json.contains("created_by") && !json.at("created_by").is_null()) {
    value.createdBy = json.at("created_by").get<int>();
  }
}

/// Читает DTO обновления таблицы purchase_order.
inline void from_json(const nlohmann::json& json, PurchaseOrderUpdateDto& value) {
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<int>();
  }
  if (json.contains("delivery_address") && !json.at("delivery_address").is_null()) {
    value.deliveryAddress = json.at("delivery_address").get<std::string>();
  }
  if (json.contains("order_date") && !json.at("order_date").is_null()) {
    value.orderDate = json.at("order_date").get<std::string>();
  }
  if (json.contains("expected_delivery_date") && !json.at("expected_delivery_date").is_null()) {
    value.expectedDeliveryDate = json.at("expected_delivery_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("total_amount") && !json.at("total_amount").is_null()) {
    value.totalAmount = json.at("total_amount").get<double>();
  }
  if (json.contains("received_at") && !json.at("received_at").is_null()) {
    value.receivedAt = json.at("received_at").get<std::string>();
  }
  if (json.contains("created_by") && !json.at("created_by").is_null()) {
    value.createdBy = json.at("created_by").get<int>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы purchase_order.
inline void to_json(nlohmann::json& json, const PurchaseOrderRowView& view) {
  json = nlohmann::json{{"resource", "purchase_order"}, {"data", view.data}};
}

/// Сериализует view списка таблицы purchase_order.
inline void to_json(nlohmann::json& json, const PurchaseOrderRowsView& view) {
  json = nlohmann::json{{"resource", "purchase_order"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы purchase_order.
inline void to_json(nlohmann::json& json, const PurchaseOrderMutationView& view) {
  json = nlohmann::json{{"resource", "purchase_order"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
