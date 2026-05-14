#pragma once

#include <marshalling/Nullable.hpp>

#include <domain/Types.hpp>

#include <controllers/dto/SalesRequisition.hpp>
#include <views/SalesRequisition.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/SalesRequisition.hpp>

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
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<std::uint64_t>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("price_per_unit") && !json.at("price_per_unit").is_null()) {
    value.pricePerUnit = json.at("price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::SalesRequisitionStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

/// Читает DTO обновления таблицы sales_requisition.
inline void from_json(const nlohmann::json& json, SalesRequisitionUpdateDto& value) {
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<std::uint64_t>();
  }
  if (json.contains("product_id") && !json.at("product_id").is_null()) {
    value.productId = json.at("product_id").get<std::uint64_t>();
  }
  if (json.contains("quantity") && !json.at("quantity").is_null()) {
    value.quantity = json.at("quantity").get<int>();
  }
  if (json.contains("price_per_unit") && !json.at("price_per_unit").is_null()) {
    value.pricePerUnit = json.at("price_per_unit").get<double>();
  }
  if (json.contains("offer_date") && !json.at("offer_date").is_null()) {
    value.offerDate = json.at("offer_date").get<fasc::server::domain::Date>();
  }
  if (json.contains("valid_until") && !json.at("valid_until").is_null()) {
    value.validUntil = json.at("valid_until").get<fasc::server::domain::Date>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<fasc::server::domain::SalesRequisitionStatus>();
  }
  if (json.contains("notes") && !json.at("notes").is_null()) {
    value.notes = json.at("notes").get<std::string>();
  }
}

} // namespace fasc::server::controllers::dto

namespace fasc::server::views {

namespace detail {

inline nlohmann::json SalesRequisitionRowPayload(const SalesRequisitionRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.farmId) {
    json["farm_id"] = *view.farmId;
  } else {
    json["farm_id"] = nullptr;
  }
  json["product_id"] = view.productId;
  json["quantity"] = view.quantity;
  if (view.pricePerUnit) {
    json["price_per_unit"] = *view.pricePerUnit;
  } else {
    json["price_per_unit"] = nullptr;
  }
  json["offer_date"] = view.offerDate;
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

inline SalesRequisitionRowView toView(const fasc::server::persistence::SalesRequisitionEntity& entity) {
  return SalesRequisitionRowView{
      entity.id,
      detail::toOptional(entity.farmId),
      entity.productId,
      entity.quantity,
      detail::toOptional(entity.pricePerUnit),
      entity.offerDate,
      detail::toOptional(entity.validUntil),
      detail::toOptional(entity.status),
      detail::toOptional(entity.notes)
  };
}

inline SalesRequisitionRowsView toView(const std::vector<fasc::server::persistence::SalesRequisitionEntity>& rows) {
  SalesRequisitionRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const SalesRequisitionRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "sales_requisition";
  json["data"] = detail::SalesRequisitionRowPayload(view);
}

inline void to_json(nlohmann::json& json, const SalesRequisitionRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "sales_requisition";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::SalesRequisitionRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const SalesRequisitionMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "sales_requisition";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
