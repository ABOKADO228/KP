#pragma once

#include <controllers/dto/AssociationEmployee.hpp>
#include <views/AssociationEmployee.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/AssociationEmployee.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы association_employee.
inline void to_json(nlohmann::json& json, const AssociationEmployeeEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["person_id"] = value.personId;
  json["association_id"] = value.associationId;
  json["role_id"] = value.roleId;
  json["employment_status_id"] = value.employmentStatusId;
  json["hire_date"] = value.hireDate;
  json["dismissal_date"] = value.dismissalDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.dismissalDate.get()};
  json["salary"] = value.salary.null() ? nlohmann::json{nullptr} : nlohmann::json{value.salary.get()};
  json["contract_number"] = value.contractNumber.null() ? nlohmann::json{nullptr} : nlohmann::json{value.contractNumber.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы association_employee.
inline void from_json(const nlohmann::json& json, AssociationEmployeeCreateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("role_id") && !json.at("role_id").is_null()) {
    value.roleId = json.at("role_id").get<int>();
  }
  if (json.contains("employment_status_id") && !json.at("employment_status_id").is_null()) {
    value.employmentStatusId = json.at("employment_status_id").get<int>();
  }
  if (json.contains("hire_date") && !json.at("hire_date").is_null()) {
    value.hireDate = json.at("hire_date").get<std::string>();
  }
  if (json.contains("dismissal_date") && !json.at("dismissal_date").is_null()) {
    value.dismissalDate = json.at("dismissal_date").get<std::string>();
  }
  if (json.contains("salary") && !json.at("salary").is_null()) {
    value.salary = json.at("salary").get<double>();
  }
  if (json.contains("contract_number") && !json.at("contract_number").is_null()) {
    value.contractNumber = json.at("contract_number").get<std::string>();
  }
}

/// Читает DTO обновления таблицы association_employee.
inline void from_json(const nlohmann::json& json, AssociationEmployeeUpdateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("association_id") && !json.at("association_id").is_null()) {
    value.associationId = json.at("association_id").get<int>();
  }
  if (json.contains("role_id") && !json.at("role_id").is_null()) {
    value.roleId = json.at("role_id").get<int>();
  }
  if (json.contains("employment_status_id") && !json.at("employment_status_id").is_null()) {
    value.employmentStatusId = json.at("employment_status_id").get<int>();
  }
  if (json.contains("hire_date") && !json.at("hire_date").is_null()) {
    value.hireDate = json.at("hire_date").get<std::string>();
  }
  if (json.contains("dismissal_date") && !json.at("dismissal_date").is_null()) {
    value.dismissalDate = json.at("dismissal_date").get<std::string>();
  }
  if (json.contains("salary") && !json.at("salary").is_null()) {
    value.salary = json.at("salary").get<double>();
  }
  if (json.contains("contract_number") && !json.at("contract_number").is_null()) {
    value.contractNumber = json.at("contract_number").get<std::string>();
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

inline nlohmann::json AssociationEmployeeRowPayload(const AssociationEmployeeRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["person_id"] = view.personId;
  json["association_id"] = view.associationId;
  json["role_id"] = view.roleId;
  json["employment_status_id"] = view.employmentStatusId;
  json["hire_date"] = view.hireDate;
  if (view.dismissalDate) {
    json["dismissal_date"] = *view.dismissalDate;
  } else {
    json["dismissal_date"] = nullptr;
  }
  if (view.salary) {
    json["salary"] = *view.salary;
  } else {
    json["salary"] = nullptr;
  }
  if (view.contractNumber) {
    json["contract_number"] = *view.contractNumber;
  } else {
    json["contract_number"] = nullptr;
  }
  return json;
}

} // namespace detail

inline AssociationEmployeeRowView toView(const fasc::server::persistence::AssociationEmployeeEntity& entity) {
  return AssociationEmployeeRowView{
      entity.id,
      entity.personId,
      entity.associationId,
      entity.roleId,
      entity.employmentStatusId,
      entity.hireDate,
      detail::toOptional(entity.dismissalDate),
      detail::toOptional(entity.salary),
      detail::toOptional(entity.contractNumber)
  };
}

inline AssociationEmployeeRowsView toView(const std::vector<fasc::server::persistence::AssociationEmployeeEntity>& rows) {
  AssociationEmployeeRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const AssociationEmployeeRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_employee";
  json["data"] = detail::AssociationEmployeeRowPayload(view);
}

inline void to_json(nlohmann::json& json, const AssociationEmployeeRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_employee";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::AssociationEmployeeRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const AssociationEmployeeMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "association_employee";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
