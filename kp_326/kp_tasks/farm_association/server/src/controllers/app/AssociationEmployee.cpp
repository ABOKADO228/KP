#include <controllers/app/AssociationEmployee.hpp>

#include <database/SqlValue.hpp>

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
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  entity.personId = fasc::server::database::requireColumn<std::uint64_t>(row, "person_id");
  entity.associationId = fasc::server::database::requireColumn<std::uint64_t>(row, "association_id");
  entity.roleId = fasc::server::database::requireColumn<std::uint64_t>(row, "role_id");
  entity.employmentStatusId = fasc::server::database::requireColumn<std::uint64_t>(row, "employment_status_id");
  entity.hireDate = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "hire_date");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::Date>(row, "dismissal_date")) {
    entity.dismissalDate = *value;
  } else {
    entity.dismissalDate.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<double>(row, "salary")) {
    entity.salary = *value;
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
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.personId));
  }
  if (!dto.associationId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.associationId));
  }
  if (!dto.roleId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: role_id"});
  }
  if (dto.roleId.has_value()) {
    columns.push_back("role_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.roleId));
  }
  if (!dto.employmentStatusId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: employment_status_id"});
  }
  if (dto.employmentStatusId.has_value()) {
    columns.push_back("employment_status_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.employmentStatusId));
  }
  if (!dto.hireDate.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: hire_date"});
  }
  if (dto.hireDate.has_value()) {
    columns.push_back("hire_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.hireDate));
  }
  if (dto.dismissalDate.has_value()) {
    columns.push_back("dismissal_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.dismissalDate));
  }
  if (dto.salary.has_value()) {
    columns.push_back("salary");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.salary));
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.contractNumber));
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.personId));
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.associationId));
  }
  if (dto.roleId.has_value()) {
    columns.push_back("role_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.roleId));
  }
  if (dto.employmentStatusId.has_value()) {
    columns.push_back("employment_status_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.employmentStatusId));
  }
  if (dto.hireDate.has_value()) {
    columns.push_back("hire_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.hireDate));
  }
  if (dto.dismissalDate.has_value()) {
    columns.push_back("dismissal_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.dismissalDate));
  }
  if (dto.salary.has_value()) {
    columns.push_back("salary");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.salary));
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.contractNumber));
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
