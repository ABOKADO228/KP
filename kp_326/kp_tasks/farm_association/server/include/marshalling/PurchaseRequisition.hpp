#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/PurchaseRequisition.hpp>
#include <views/PurchaseRequisition.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/PurchaseRequisition.hpp>

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
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<std::uint64_t>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("max_price_per_unit") && !json.at("max_price_per_unit").is_null()) {
    value.maxPricePerUnit = json.at("max_price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("required_date") && !json.at("required_date").is_null()) {
    value.requiredDate = json.at("required_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("priority") && !json.at("priority").is_null()) {
    value.priority = json.at("priority").get<int>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::PurchaseRequisitionStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы purchase_requisition.
inline void from_json(const nlohmann::json& json, PurchaseRequisitionUpdateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<std::uint64_t>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("max_price_per_unit") && !json.at("max_price_per_unit").is_null()) {
    value.maxPricePerUnit = json.at("max_price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("required_date") && !json.at("required_date").is_null()) {
    value.requiredDate = json.at("required_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("priority") && !json.at("priority").is_null()) {
    value.priority = json.at("priority").get<int>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::PurchaseRequisitionStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json PurchaseRequisitionRowPayload(const PurchaseRequisitionRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["farm_id"] = view.farmId;
  json["product_id"] = view.productId;
  json["quantity"] = view.quantity;
  if (view.maxPricePerUnit) {
    json["max_price_per_unit"] = *view.maxPricePerUnit;
  } else {
    json["max_price_per_unit"] = nullptr;
  }
  if (view.offerDate) {
    json["offer_date"] = *view.offerDate;
  } else {
    json["offer_date"] = nullptr;
  }
  if (view.requiredDate) {
    json["required_date"] = *view.requiredDate;
  } else {
    json["required_date"] = nullptr;
  }
  if (view.priority) {
    json["priority"] = *view.priority;
  } else {
    json["priority"] = nullptr;
  }
  if (view.validUntil) {
    json["valid_until"] = *view.validUntil;
  } else {
    json["valid_until"] = nullptr;
  }
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.notes) {
    json["notes"] = *view.notes;
  } else {
    json["notes"] = nullptr;
  }
  return json;
}

} // namespace detail

inline PurchaseRequisitionRowView toView(const fasc::server::persistence::PurchaseRequisitionEntity& entity) {
  return PurchaseRequisitionRowView{
      entity.id,
      entity.farmId,
      entity.productId,
      entity.quantity,
      detail::toOptional(entity.maxPricePerUnit),
      detail::toOptional(entity.offerDate),
      detail::toOptional(entity.requiredDate),
      detail::toOptional(entity.priority),
      detail::toOptional(entity.validUntil),
      detail::toOptional(entity.status),
      detail::toOptional(entity.notes)
  };
}

inline PurchaseRequisitionRowsView toView(const std::vector<fasc::server::persistence::PurchaseRequisitionEntity>& rows) {
  PurchaseRequisitionRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const PurchaseRequisitionRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_requisition";
  json["data"] = detail::PurchaseRequisitionRowPayload(view);
}

inline void to_json(nlohmann::json& json, const PurchaseRequisitionRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_requisition";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::PurchaseRequisitionRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const PurchaseRequisitionMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "purchase_requisition";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
