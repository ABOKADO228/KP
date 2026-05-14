#pragma once

#include <controllers/dto/FarmEmployee.hpp>
#include <views/FarmEmployee.hpp>
#include <optional>
#include <odb/nullable.hxx>
#include <persistence/FarmEmployee.hpp>

#include <nlohmann/json.hpp>

namespace fasc::server::persistence {

/// Сериализует сущность таблицы farm_employee.
inline void to_json(nlohmann::json& json, const FarmEmployeeEntity& value) {
  json = nlohmann::json::object();
  json["id"] = value.id;
  json["person_id"] = value.personId;
  json["farm_id"] = value.farmId;
  json["role_id"] = value.roleId;
  json["employment_status_id"] = value.employmentStatusId;
  json["hire_date"] = value.hireDate;
  json["dismissal_date"] = value.dismissalDate.null() ? nlohmann::json{nullptr} : nlohmann::json{value.dismissalDate.get()};
  json["salary"] = value.salary.null() ? nlohmann::json{nullptr} : nlohmann::json{value.salary.get()};
  json["employment_contract_number"] = value.employmentContractNumber.null() ? nlohmann::json{nullptr} : nlohmann::json{value.employmentContractNumber.get()};
  json["is_primary_workplace"] = value.isPrimaryWorkplace.null() ? nlohmann::json{nullptr} : nlohmann::json{value.isPrimaryWorkplace.get()};
}

} // namespace fasc::server::persistence

namespace fasc::server::controllers::dto {

/// Читает DTO создания таблицы farm_employee.
inline void from_json(const nlohmann::json& json, FarmEmployeeCreateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
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
  if (json.contains("employment_contract_number") && !json.at("employment_contract_number").is_null()) {
    value.employmentContractNumber = json.at("employment_contract_number").get<std::string>();
  }
  if (json.contains("is_primary_workplace") && !json.at("is_primary_workplace").is_null()) {
    value.isPrimaryWorkplace = json.at("is_primary_workplace").get<int>();
  }
}

/// Читает DTO обновления таблицы farm_employee.
inline void from_json(const nlohmann::json& json, FarmEmployeeUpdateDto& value) {
  if (json.contains("person_id") && !json.at("person_id").is_null()) {
    value.personId = json.at("person_id").get<int>();
  }
  if (json.contains("farm_id") && !json.at("farm_id").is_null()) {
    value.farmId = json.at("farm_id").get<int>();
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
  if (json.contains("employment_contract_number") && !json.at("employment_contract_number").is_null()) {
    value.employmentContractNumber = json.at("employment_contract_number").get<std::string>();
  }
  if (json.contains("is_primary_workplace") && !json.at("is_primary_workplace").is_null()) {
    value.isPrimaryWorkplace = json.at("is_primary_workplace").get<int>();
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

inline nlohmann::json FarmEmployeeRowPayload(const FarmEmployeeRowView& view) {
  nlohmann::json json = nlohmann::json::object();
  json["id"] = view.id;
  json["person_id"] = view.personId;
  json["farm_id"] = view.farmId;
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
  if (view.employmentContractNumber) {
    json["employment_contract_number"] = *view.employmentContractNumber;
  } else {
    json["employment_contract_number"] = nullptr;
  }
  if (view.isPrimaryWorkplace) {
    json["is_primary_workplace"] = *view.isPrimaryWorkplace;
  } else {
    json["is_primary_workplace"] = nullptr;
  }
  return json;
}

} // namespace detail

inline FarmEmployeeRowView toView(const fasc::server::persistence::FarmEmployeeEntity& entity) {
  return FarmEmployeeRowView{
      entity.id,
      entity.personId,
      entity.farmId,
      entity.roleId,
      entity.employmentStatusId,
      entity.hireDate,
      detail::toOptional(entity.dismissalDate),
      detail::toOptional(entity.salary),
      detail::toOptional(entity.employmentContractNumber),
      detail::toOptional(entity.isPrimaryWorkplace)
  };
}

inline FarmEmployeeRowsView toView(const std::vector<fasc::server::persistence::FarmEmployeeEntity>& rows) {
  FarmEmployeeRowsView view;
  view.rows.reserve(rows.size());
  for (const auto& row : rows) {
    view.rows.push_back(toView(row));
  }
  return view;
}

inline void to_json(nlohmann::json& json, const FarmEmployeeRowView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_employee";
  json["data"] = detail::FarmEmployeeRowPayload(view);
}

inline void to_json(nlohmann::json& json, const FarmEmployeeRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_employee";
  nlohmann::json rows = nlohmann::json::array();
  for (const auto& row : view.rows) {
    rows.push_back(detail::FarmEmployeeRowPayload(row));
  }
  json["rows"] = rows;
}

inline void to_json(nlohmann::json& json, const FarmEmployeeMutationView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm_employee";
  json["affectedRows"] = view.affectedRows;
}

} // namespace fasc::server::views
