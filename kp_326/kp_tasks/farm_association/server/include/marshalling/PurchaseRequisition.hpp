#pragma once

#include <controllers/dto/PurchaseRequisition.hpp>
#include <views/PurchaseRequisition.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы purchase_requisition.
inline void to_json(nlohmann::json& json, const PurchaseRequisitionEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["farm_id"] = value.farmId;
  json["product_id"] = value.productId;
  json["quantity"] = value.quantity;
  json["max_price_per_unit"] = value.maxPricePerUnit.null() ? nlohmann::json{nullptr} : nlohmann::json{value.maxPricePerUnit.get()};
  json["offer_date"] = value.offerDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.offerDate.get()};
  json["required_date"] = value.requiredDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.requiredDate.get()};
  json["priority"] = value.priority.null() ? nlohmann::json{nullptr} : nlohmann::json{value.priority.get()};
  json["valid_until"] = value.validUntil.null() ? nlohmann::json{nullptr} : nlohmann::json{value.validUntil.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["notes"] = value.notes.null() ? nlohmann::json{nullptr} : nlohmann::json{value.notes.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы purchase_requisition.
inline void from_json(const nlohmann::json& json, PurchaseRequisitionCreateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("max_price_per_unit") && !json.at("max_price_per_unit").is_null()) {
    value.maxPricePerUnit = json.at("max_price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<std::string>();
  }
  if (json.contains("required_date") && !json.at("required_date").is_null()) {
    value.requiredDate = json.at("required_date").get<std::string>();
  }
  if (json.contains("priority") && !json.at("priority").is_null()) {
    value.priority = json.at("priority").get<int>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы purchase_requisition.
inline void from_json(const nlohmann::json& json, PurchaseRequisitionUpdateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("max_price_per_unit") && !json.at("max_price_per_unit").is_null()) {
    value.maxPricePerUnit = json.at("max_price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<std::string>();
  }
  if (json.contains("required_date") && !json.at("required_date").is_null()) {
    value.requiredDate = json.at("required_date").get<std::string>();
  }
  if (json.contains("priority") && !json.at("priority").is_null()) {
    value.priority = json.at("priority").get<int>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

/// Сериализует view строки таблицы purchase_requisition.
inline void to_json(nlohmann::json& json, const PurchaseRequisitionRowView& view) {
  json = nlohmann::json{{"resource", "purchase_requisition"}, {"data", view.data}};
}

/// Сериализует view списка таблицы purchase_requisition.
inline void to_json(nlohmann::json& json, const PurchaseRequisitionRowsView& view) {
  json = nlohmann::json{{"resource", "purchase_requisition"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы purchase_requisition.
inline void to_json(nlohmann::json& json, const PurchaseRequisitionMutationView& view) {
  json = nlohmann::json{{"resource", "purchase_requisition"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
