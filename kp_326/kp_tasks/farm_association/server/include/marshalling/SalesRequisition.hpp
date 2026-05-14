#pragma once

#include <controllers/dto/SalesRequisition.hpp>
#include <views/SalesRequisition.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы sales_requisition.
inline void to_json(nlohmann::json& json, const SalesRequisitionEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["farm_id"] = value.farmId.null() ? nlohmann::json{nullptr} : nlohmann::json{value.farmId.get()};
  json["product_id"] = value.productId;
  json["quantity"] = value.quantity;
  json["price_per_unit"] = value.pricePerUnit.null() ? nlohmann::json{nullptr} : nlohmann::json{value.pricePerUnit.get()};
  json["offer_date"] = value.offerDate;
  json["valid_until"] = value.validUntil.null() ? nlohmann::json{nullptr} : nlohmann::json{value.validUntil.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["notes"] = value.notes.null() ? nlohmann::json{nullptr} : nlohmann::json{value.notes.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы sales_requisition.
inline void from_json(const nlohmann::json& json, SalesRequisitionCreateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("price_per_unit") && !json.at("price_per_unit").is_null()) {
    value.pricePerUnit = json.at("price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<std::string>();
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

/// Читает DTO обновления таблицы sales_requisition.
inline void from_json(const nlohmann::json& json, SalesRequisitionUpdateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<int>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("price_per_unit") && !json.at("price_per_unit").is_null()) {
    value.pricePerUnit = json.at("price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<std::string>();
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

/// Сериализует view строки таблицы sales_requisition.
inline void to_json(nlohmann::json& json, const SalesRequisitionRowView& view) {
  json = nlohmann::json{{"resource", "sales_requisition"}, {"data", view.data}};
}

/// Сериализует view списка таблицы sales_requisition.
inline void to_json(nlohmann::json& json, const SalesRequisitionRowsView& view) {
  json = nlohmann::json{{"resource", "sales_requisition"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы sales_requisition.
inline void to_json(nlohmann::json& json, const SalesRequisitionMutationView& view) {
  json = nlohmann::json{{"resource", "sales_requisition"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
