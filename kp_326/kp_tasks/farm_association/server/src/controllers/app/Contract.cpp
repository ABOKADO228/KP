#include <controllers/app/Contract.hpp>

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

std::string columnsSql(const std::vector<std::string>& columns) {
  std::string sql;
  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i != 0) {
      sql += ", ";
    }
    sql += columns[i];
  }
  return sql;
}

std::string whereSql(const std::vector<std::string>& keys, std::size_t offset = 0) {
  std::string sql;
  for (std::size_t i = 0; i < keys.size(); ++i) {
    if (i != 0) {
      sql += " AND ";
    }
    sql += keys[i] + " = $" + std::to_string(i + 1 + offset);
  }
  return sql;
}

fasc::server::persistence::ContractEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::ContractEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "supplier_id")) {
    entity.supplierId = std::stoi(*value);
  } else {
    entity.supplierId.reset();
  }
  if (const auto value = optionalColumn(row, "farm_id")) {
    entity.farmId = std::stoi(*value);
  } else {
    entity.farmId.reset();
  }
  entity.associationId = std::stoi(requireColumn(row, "association_id"));
  if (const auto value = optionalColumn(row, "contract_number")) {
    entity.contractNumber = *value;
  } else {
    entity.contractNumber.reset();
  }
  entity.signDate = requireColumn(row, "sign_date");
  entity.startDate = requireColumn(row, "start_date");
  if (const auto value = optionalColumn(row, "end_date")) {
    entity.endDate = *value;
  } else {
    entity.endDate.reset();
  }
  if (const auto value = optionalColumn(row, "status")) {
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
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

ContractController::ContractController(fasc::server::database::Database& db) : db_(db) {}

ContractRowsResult ContractController::list() const {
  static const std::vector<std::string> columns{"id", "supplier_id", "farm_id", "association_id", "contract_number", "sign_date", "start_date", "end_date", "status", "description"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.contract";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
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
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.contract WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return ContractRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ContractRowResult::success(
        fasc::server::controllers::dto::ContractRowDto{rowToEntity(rows.front())});
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
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.supplierId), false});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (!dto.associationId.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.contractNumber, false});
  }
  if (!dto.signDate.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: sign_date"});
  }
  if (dto.signDate.has_value()) {
    columns.push_back("sign_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.signDate, false});
  }
  if (!dto.startDate.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: start_date"});
  }
  if (dto.startDate.has_value()) {
    columns.push_back("start_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.startDate, false});
  }
  if (dto.endDate.has_value()) {
    columns.push_back("end_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.endDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::SqlParameter{*dto.description, false});
  }
  if (columns.empty()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.contract (" + columnsSql(columns) + ") VALUES (";
    for (std::size_t i = 0; i < values.size(); ++i) {
      if (i != 0) {
        sql += ", ";
      }
      sql += "$" + std::to_string(i + 1);
    }
    sql += ")";
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
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
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.supplierId), false});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (dto.contractNumber.has_value()) {
    columns.push_back("contract_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.contractNumber, false});
  }
  if (dto.signDate.has_value()) {
    columns.push_back("sign_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.signDate, false});
  }
  if (dto.startDate.has_value()) {
    columns.push_back("start_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.startDate, false});
  }
  if (dto.endDate.has_value()) {
    columns.push_back("end_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.endDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::SqlParameter{*dto.description, false});
  }
  if (columns.empty()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.contract SET ";
    for (std::size_t i = 0; i < columns.size(); ++i) {
      if (i != 0) {
        sql += ", ";
      }
      sql += columns[i] + " = $" + std::to_string(i + 1);
    }
    sql += " WHERE " + whereSql(keys, values.size());
    const std::vector<fasc::server::database::SqlParameter> keyParams = keyValues(key);
    values.insert(values.end(), keyParams.begin(), keyParams.end());
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
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
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.contract WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return ContractMutationResult::success(
        fasc::server::controllers::dto::ContractMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
