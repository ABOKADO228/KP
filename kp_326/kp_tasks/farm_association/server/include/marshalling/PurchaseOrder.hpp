#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/PurchaseOrder.hpp>
#include <views/PurchaseOrder.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/PurchaseOrder.hpp>

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
    value.associationId = json.at("association_id").get<std::uint64_t>();
  }
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<std::uint64_t>();
  }
  if (json.contains("delivery_address") && !json.at("delivery_address").is_null()) {
    value.deliveryAddress = json.at("delivery_address").get<std::string>();
  }
  if (json.contains("order_date") && !json.at("order_date").is_null()) {
    value.orderDate = json.at("order_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("expected_delivery_date") && !json.at("expected_delivery_date").is_null()) {
    value.expectedDeliveryDate = json.at("expected_delivery_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::PurchaseOrderStatus>();
  }
  if (json.contains("total_amount") && !json.at("total_amount").is_null()) {
    value.totalAmount = json.at("total_amount").get<double>();
  }
  if (json.contains("received_at") && !json.at("received_at").is_null()) {
    value.receivedAt = json.at("received_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("created_by") && !json.at("created_by").is_null()) {
    value.createdBy = json.at("created_by").get<std::uint64_t>();
  }
}

/// Читает DTO обновления таблицы purchase_order.
inline void from_json(const nlohmann::json& json, PurchaseOrderUpdateDto& value) {
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<std::uint64_t>();
  }
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<std::uint64_t>();
  }
  if (json.contains("delivery_address") && !json.at("delivery_address").is_null()) {
    value.deliveryAddress = json.at("delivery_address").get<std::string>();
  }
  if (json.contains("order_date") && !json.at("order_date").is_null()) {
    value.orderDate = json.at("order_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("expected_delivery_date") && !json.at("expected_delivery_date").is_null()) {
    value.expectedDeliveryDate = json.at("expected_delivery_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::PurchaseOrderStatus>();
  }
  if (json.contains("total_amount") && !json.at("total_amount").is_null()) {
    value.totalAmount = json.at("total_amount").get<double>();
  }
  if (json.contains("received_at") && !json.at("received_at").is_null()) {
    value.receivedAt = json.at("received_at").get<fasc::server::domain::Date>();
  }
  if (json.contains("created_by") && !json.at("created_by").is_null()) {
    value.createdBy = json.at("created_by").get<std::uint64_t>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json PurchaseOrderRowPayload(const PurchaseOrderRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["association_id"] = view.associationId;
  json["supplier_id"] = view.supplierId;
  if (view.deliveryAddress) {
    json["delivery_address"] = *view.deliveryAddress;
  } else {
    json["delivery_address"] = nullptr;
  }
  json["order_date"] = view.orderDate;
  if (view.expectedDeliveryDate) {
    json["expected_delivery_date"] = *view.expectedDeliveryDate;
  } else {
    json["expected_delivery_date"] = nullptr;
  }
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.totalAmount) {
    json["total_amount"] = *view.totalAmount;
  } else {
    json["total_amount"] = nullptr;
  }
  if (view.receivedAt) {
    json["received_at"] = *view.receivedAt;
  } else {
    json["received_at"] = nullptr;
  }
  if (view.createdBy) {
    json["created_by"] = *view.createdBy;
  } else {
    json["created_by"] = nullptr;
  }
  return json;
}

} // namespace detail

inline PurchaseOrderRowView toView(const fasc::server::persistence::PurchaseOrderEntity& entity) {
  return PurchaseOrderRowView{
      entity.id,
      entity.associationId,
      entity.supplierId,
      detail::toOptional(entity.deliveryAddress),
      entity.orderDate,
      detail::toOptional(entity.expectedDeliveryDate),
      detail::toOptional(entity.status),
      detail::toOptional(entity.totalAmount),
      detail::toOptional(entity.receivedAt),
      detail::toOptional(entity.createdBy)
  };
}

inline PurchaseOrderRowsView toView(const std::vector<fasc::server::persistence::PurchaseOrderEntity>& rows) {
  PurchaseOrderRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const PurchaseOrderRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_order";
  json["data"] = detail::PurchaseOrderRowPayload(view);
}

inline void to_json(nlohmann::json& json, const PurchaseOrderRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_order";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::PurchaseOrderRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const PurchaseOrderMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_order";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
