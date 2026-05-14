#include <controllers/app/Contract.hpp>

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


fasc::server::persistence::ContractEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::ContractEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  if (const auto value = fasc::server::database::optionalColumn<std::uint64_t>(row, "supplier_id")) {
    entity.supplierId = *value;
  } else {
    entity.supplierId.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<std::uint64_t>(row, "farm_id")) {
    entity.farmId = *value;
  } else {
    entity.farmId.reset();
  }
  entity.associationId = fasc::server::database::requireColumn<std::uint64_t>(row, "association_id");
  if (const auto value = optionalColumn(row, "contract_number")) {
    entity.contractNumber = *value;
  } else {
    entity.contractNumber.reset();
  }
  entity.signDate = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "sign_date");
  entity.startDate = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "start_date");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::Date>(row, "end_date")) {
    entity.endDate = *value;
  } else {
    entity.endDate.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::ContractStatus>(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "description")) {
    entity.description = *value;
  } else {
    entity.description.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::ContractKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

ContractController::ContractController(fasc::server::database::Database& db) : db_(db) {}

ContractRowsResult ContractController::list() const {
  static const std::vector<std::string> columns{"id", "supplier_id", "farm_id", "association_id", "contract_number", "sign_date", "start_date", "end_date", "status", "description"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.contract", columns);
    });

    fasc::server::controllers::dto::ContractRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return ContractRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ContractRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractRowResult ContractController::load(const fasc::server::controllers::dto::ContractKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "supplier_id", "farm_id", "association_id", "contract_number", "sign_date", "start_date", "end_date", "status", "description"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.contract", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return ContractRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ContractRowResult::success(
        fasc::server::controllers::dto::ContractRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return ContractRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractMutationResult ContractController::create(
    const fasc::server::controllers::dto::ContractCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.supplierId.has_value()) {
    columns.push_back("supplier_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.supplierId));
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmId));
  }
  if (!dto.associationId.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.associationId));
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.contractNumber));
  }
  if (!dto.signDate.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: sign_date"});
  }
  if (dto.signDate.has_value()) {
    columns.push_back("sign_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.signDate));
  }
  if (!dto.startDate.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: start_date"});
  }
  if (dto.startDate.has_value()) {
    columns.push_back("start_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.startDate));
  }
  if (dto.endDate.has_value()) {
    columns.push_back("end_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.endDate));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.description));
  }
  if (columns.empty()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.contract", columns, values);
    });
    return ContractMutationResult::success(
        fasc::server::controllers::dto::ContractMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractMutationResult ContractController::update(
    const fasc::server::controllers::dto::ContractKeyDto& key,
    const fasc::server::controllers::dto::ContractUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.supplierId.has_value()) {
    columns.push_back("supplier_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.supplierId));
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmId));
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.associationId));
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.contractNumber));
  }
  if (dto.signDate.has_value()) {
    columns.push_back("sign_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.signDate));
  }
  if (dto.startDate.has_value()) {
    columns.push_back("start_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.startDate));
  }
  if (dto.endDate.has_value()) {
    columns.push_back("end_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.endDate));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.description));
  }
  if (columns.empty()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.contract", columns, values, keys, keyValues(key));
    });
    return ContractMutationResult::success(
        fasc::server::controllers::dto::ContractMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractMutationResult ContractController::erase(
    const fasc::server::controllers::dto::ContractKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.contract", keys, keyValues(key));
    });
    return ContractMutationResult::success(
        fasc::server::controllers::dto::ContractMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
