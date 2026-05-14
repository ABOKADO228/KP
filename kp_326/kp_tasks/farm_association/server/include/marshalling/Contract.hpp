#pragma once

#include <controllers/dto/Contract.hpp>
#include <views/Contract.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/Contract.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы contract.
inline void to_json(nlohmann::json& json, const ContractEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["supplier_id"] = value.supplierId.null() ? nlohmann::json{nullptr} : nlohmann::json{value.supplierId.get()};
  json["farm_id"] = value.farmId.null() ? nlohmann::json{nullptr} : nlohmann::json{value.farmId.get()};
  json["association_id"] = value.associationId;
  json["contract_number"] = value.contractNumber.null() ? nlohmann::json{nullptr} : nlohmann::json{value.contractNumber.get()};
  json["sign_date"] = value.signDate;
  json["start_date"] = value.startDate;
  json["end_date"] = value.endDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.endDate.get()};
  json["status"] = value.status.null() ? nlohmann::json{nullptr} : nlohmann::json{value.status.get()};
  json["description"] = value.description.null() ? nlohmann::json{nullptr} : nlohmann::json{value.description.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы contract.
inline void from_json(const nlohmann::json& json, ContractCreateDto& value) {
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<int>();
  }
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("contract_number") && !json.at("contract_number").is_null()) {
    value.contractNumber = json.at("contract_number").get<std::string>();
  }
  if (json.contains("sign_date") && !json.at("sign_date").is_null()) {
    value.signDate = json.at("sign_date").get<std::string>();
  }
  if (json.contains("start_date") && !json.at("start_date").is_null()) {
    value.startDate = json.at("start_date").get<std::string>();
  }
  if (json.contains("end_date") && !json.at("end_date").is_null()) {
    value.endDate = json.at("end_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
  }
}

/// Читает DTO обновления таблицы contract.
inline void from_json(const nlohmann::json& json, ContractUpdateDto& value) {
  if (json.contains("supplier_id") && !json.at("supplier_id").is_null()) {
    value.supplierId = json.at("supplier_id").get<int>();
  }
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
  }
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("contract_number") && !json.at("contract_number").is_null()) {
    value.contractNumber = json.at("contract_number").get<std::string>();
  }
  if (json.contains("sign_date") && !json.at("sign_date").is_null()) {
    value.signDate = json.at("sign_date").get<std::string>();
  }
  if (json.contains("start_date") && !json.at("start_date").is_null()) {
    value.startDate = json.at("start_date").get<std::string>();
  }
  if (json.contains("end_date") && !json.at("end_date").is_null()) {
    value.endDate = json.at("end_date").get<std::string>();
  }
  if (json.contains("status") && !json.at("status").is_null()) {
    value.status = json.at("status").get<std::string>();
  }
  if (json.contains("description") && !json.at("description").is_null()) {
    value.description = json.at("description").get<std::string>();
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

inline nlohmann::json ContractRowPayload(const ContractRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  if (view.supplierId) {
    json["supplier_id"] = *view.supplierId;
  } else {
    json["supplier_id"] = nullptr;
  }
  if (view.farmId) {
    json["farm_id"] = *view.farmId;
  } else {
    json["farm_id"] = nullptr;
  }
  json["association_id"] = view.associationId;
  if (view.contractNumber) {
    json["contract_number"] = *view.contractNumber;
  } else {
    json["contract_number"] = nullptr;
  }
  json["sign_date"] = view.signDate;
  json["start_date"] = view.startDate;
  if (view.endDate) {
    json["end_date"] = *view.endDate;
  } else {
    json["end_date"] = nullptr;
  }
  if (view.status) {
    json["status"] = *view.status;
  } else {
    json["status"] = nullptr;
  }
  if (view.description) {
    json["description"] = *view.description;
  } else {
    json["description"] = nullptr;
  }
  return json;
}

} // namespace detail

inline ContractRowView toView(const fasc::server::persistence::ContractEntity& entity) {
  return ContractRowView{
      entity.id,
      detail::toOptional(entity.supplierId),
      detail::toOptional(entity.farmId),
      entity.associationId,
      detail::toOptional(entity.contractNumber),
      entity.signDate,
      entity.startDate,
      detail::toOptional(entity.endDate),
      detail::toOptional(entity.status),
      detail::toOptional(entity.description)
  };
}

inline ContractRowsView toView(const std::vector<fasc::server::persistence::ContractEntity>& rows) {
  ContractRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const ContractRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "contract";
  json["data"] = detail::ContractRowPayload(view);
}

inline void to_json(nlohmann::json& json, const ContractRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "contract";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::ContractRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const ContractMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "contract";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
