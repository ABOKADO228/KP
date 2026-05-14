#pragma once

#include <controllers/dto/AssociationEmployee.hpp>
#include <views/AssociationEmployee.hpp>

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

/// Сериализует view строки таблицы association_employee.
inline void to_json(nlohmann::json& json, const AssociationEmployeeRowView& view) {
  json = nlohmann::json{{"resource", "association_employee"}, {"data", view.data}};
}

/// Сериализует view списка таблицы association_employee.
inline void to_json(nlohmann::json& json, const AssociationEmployeeRowsView& view) {
  json = nlohmann::json{{"resource", "association_employee"}, {"rows", view.rows}};
}

/// Сериализует view изменения таблицы association_employee.
inline void to_json(nlohmann::json& json, const AssociationEmployeeMutationView& view) {
  json = nlohmann::json{{"resource", "association_employee"}, {"affectedRows", view.affectedRows}};
}

} // namespace fasc::server::views
