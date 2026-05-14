#include <controllers/app/FarmEmployee.hpp>

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


fasc::server::persistence::FarmEmployeeEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmEmployeeEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.personId = std::stoi(requireColumn(row, "person_id"));
  entity.farmId = std::stoi(requireColumn(row, "farm_id"));
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
  if (const auto value = optionalColumn(row, "employment_contract_number")) {
    entity.employmentContractNumber = *value;
  } else {
    entity.employmentContractNumber.reset();
  }
  if (const auto value = optionalColumn(row, "is_primary_workplace")) {
    entity.isPrimaryWorkplace = std::stoi(*value);
  } else {
    entity.isPrimaryWorkplace.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmEmployeeController::FarmEmployeeController(fasc::server::database::Database& db) : db_(db) {}

FarmEmployeeRowsResult FarmEmployeeController::list() const {
  static const std::vector<std::string> columns{"id", "person_id", "farm_id", "role_id", "employment_status_id", "hire_date", "dismissal_date", "salary", "employment_contract_number", "is_primary_workplace"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_employee", columns);
    });

    fasc::server::controllers::dto::FarmEmployeeRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmEmployeeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmEmployeeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeRowResult FarmEmployeeController::load(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "person_id", "farm_id", "role_id", "employment_status_id", "hire_date", "dismissal_date", "salary", "employment_contract_number", "is_primary_workplace"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_employee", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmEmployeeRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmEmployeeRowResult::success(
        fasc::server::controllers::dto::FarmEmployeeRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmEmployeeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeMutationResult FarmEmployeeController::create(
    const fasc::server::controllers::dto::FarmEmployeeCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.personId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (!dto.farmId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (!dto.roleId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: role_id"});
  }
  if (dto.roleId.has_value()) {
    columns.push_back("role_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.roleId), false});
  }
  if (!dto.employmentStatusId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: employment_status_id"});
  }
  if (dto.employmentStatusId.has_value()) {
    columns.push_back("employment_status_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.employmentStatusId), false});
  }
  if (!dto.hireDate.has_value()) {
    return FarmEmployeeMutationResult::failure(
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
  if (dto.employmentContractNumber.has_value()) {
    columns.push_back("employment_contract_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.employmentContractNumber, false});
  }
  if (dto.isPrimaryWorkplace.has_value()) {
    columns.push_back("is_primary_workplace");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.isPrimaryWorkplace), false});
  }
  if (columns.empty()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_employee", columns, values);
    });
    return FarmEmployeeMutationResult::success(
        fasc::server::controllers::dto::FarmEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeMutationResult FarmEmployeeController::update(
    const fasc::server::controllers::dto::FarmEmployeeKeyDto& key,
    const fasc::server::controllers::dto::FarmEmployeeUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
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
  if (dto.employmentContractNumber.has_value()) {
    columns.push_back("employment_contract_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.employmentContractNumber, false});
  }
  if (dto.isPrimaryWorkplace.has_value()) {
    columns.push_back("is_primary_workplace");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.isPrimaryWorkplace), false});
  }
  if (columns.empty()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_employee", columns, values, keys, keyValues(key));
    });
    return FarmEmployeeMutationResult::success(
        fasc::server::controllers::dto::FarmEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeMutationResult FarmEmployeeController::erase(
    const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_employee", keys, keyValues(key));
    });
    return FarmEmployeeMutationResult::success(
        fasc::server::controllers::dto::FarmEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
