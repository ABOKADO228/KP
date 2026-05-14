#include <controllers/app/AssociationEmployee.hpp>

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

std::string requireColumn(const fasc::server::database::SqlRow& row, const std::string& column) {
  const auto it = row.find(column);
  if (it == row.end() || !it->second.has_value()) {
    throw std::runtime_error{"Column is null: " + column};
  }
  return *it->second;
}

std::optional<std::string> optionalColumn(const fasc::server::database::SqlRow& row,
                                         const std::string& column) {
  const auto it = row.find(column);
  if (it == row.end()) {
    throw std::runtime_error{"Column is missing: " + column};
  }
  return it->second;
}


fasc::server::persistence::AssociationEmployeeEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::AssociationEmployeeEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.personId = std::stoi(requireColumn(row, "person_id"));
  entity.associationId = std::stoi(requireColumn(row, "association_id"));
  entity.roleId = std::stoi(requireColumn(row, "role_id"));
  entity.employmentStatusId = std::stoi(requireColumn(row, "employment_status_id"));
  entity.hireDate = requireColumn(row, "hire_date");
  if (const auto value = optionalColumn(row, "dismissal_date")) {
    entity.dismissalDate = *value;
  } else {
    entity.dismissalDate.reset();
  }
  if (const auto value = optionalColumn(row, "salary")) {
    entity.salary = std::stod(*value);
  } else {
    entity.salary.reset();
  }
  if (const auto value = optionalColumn(row, "contract_number")) {
    entity.contractNumber = *value;
  } else {
    entity.contractNumber.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

AssociationEmployeeController::AssociationEmployeeController(fasc::server::database::Database& db) : db_(db) {}

AssociationEmployeeRowsResult AssociationEmployeeController::list() const {
  static const std::vector<std::string> columns{"id", "person_id", "association_id", "role_id", "employment_status_id", "hire_date", "dismissal_date", "salary", "contract_number"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.association_employee", columns);
    });

    fasc::server::controllers::dto::AssociationEmployeeRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return AssociationEmployeeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationEmployeeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeRowResult AssociationEmployeeController::load(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "person_id", "association_id", "role_id", "employment_status_id", "hire_date", "dismissal_date", "salary", "contract_number"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.association_employee", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return AssociationEmployeeRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationEmployeeRowResult::success(
        fasc::server::controllers::dto::AssociationEmployeeRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return AssociationEmployeeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeMutationResult AssociationEmployeeController::create(
    const fasc::server::controllers::dto::AssociationEmployeeCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.personId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (!dto.associationId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (!dto.roleId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: role_id"});
  }
  if (dto.roleId.has_value()) {
    columns.push_back("role_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.roleId), false});
  }
  if (!dto.employmentStatusId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: employment_status_id"});
  }
  if (dto.employmentStatusId.has_value()) {
    columns.push_back("employment_status_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.employmentStatusId), false});
  }
  if (!dto.hireDate.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: hire_date"});
  }
  if (dto.hireDate.has_value()) {
    columns.push_back("hire_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.hireDate, false});
  }
  if (dto.dismissalDate.has_value()) {
    columns.push_back("dismissal_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.dismissalDate, false});
  }
  if (dto.salary.has_value()) {
    columns.push_back("salary");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.salary), false});
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.contractNumber, false});
  }
  if (columns.empty()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.association_employee", columns, values);
    });
    return AssociationEmployeeMutationResult::success(
        fasc::server::controllers::dto::AssociationEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeMutationResult AssociationEmployeeController::update(
    const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key,
    const fasc::server::controllers::dto::AssociationEmployeeUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (dto.roleId.has_value()) {
    columns.push_back("role_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.roleId), false});
  }
  if (dto.employmentStatusId.has_value()) {
    columns.push_back("employment_status_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.employmentStatusId), false});
  }
  if (dto.hireDate.has_value()) {
    columns.push_back("hire_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.hireDate, false});
  }
  if (dto.dismissalDate.has_value()) {
    columns.push_back("dismissal_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.dismissalDate, false});
  }
  if (dto.salary.has_value()) {
    columns.push_back("salary");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.salary), false});
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.contractNumber, false});
  }
  if (columns.empty()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.association_employee", columns, values, keys, keyValues(key));
    });
    return AssociationEmployeeMutationResult::success(
        fasc::server::controllers::dto::AssociationEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeMutationResult AssociationEmployeeController::erase(
    const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.association_employee", keys, keyValues(key));
    });
    return AssociationEmployeeMutationResult::success(
        fasc::server::controllers::dto::AssociationEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
