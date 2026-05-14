#include <controllers/app/FarmOwnership.hpp>

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

fasc::server::persistence::FarmOwnershipEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmOwnershipEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.farmId = std::stoi(requireColumn(row, "farm_id"));
  entity.farmOwnerId = std::stoi(requireColumn(row, "farm_owner_id"));
  if (const auto value = optionalColumn(row, "ownership_percentage")) {
    entity.ownershipPercentage = std::stod(*value);
  } else {
    entity.ownershipPercentage.reset();
  }
  entity.startedAt = requireColumn(row, "started_at");
  if (const auto value = optionalColumn(row, "ended_at")) {
    entity.endedAt = *value;
  } else {
    entity.endedAt.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmOwnershipController::FarmOwnershipController(fasc::server::database::Database& db) : db_(db) {}

FarmOwnershipRowsResult FarmOwnershipController::list() const {
  static const std::vector<std::string> columns{"id", "farm_id", "farm_owner_id", "ownership_percentage", "started_at", "ended_at"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_ownership";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::FarmOwnershipRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmOwnershipRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmOwnershipRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipRowResult FarmOwnershipController::load(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "farm_id", "farm_owner_id", "ownership_percentage", "started_at", "ended_at"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_ownership WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return FarmOwnershipRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmOwnershipRowResult::success(
        fasc::server::controllers::dto::FarmOwnershipRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return FarmOwnershipRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipMutationResult FarmOwnershipController::create(
    const fasc::server::controllers::dto::FarmOwnershipCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.farmId.has_value()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (!dto.farmOwnerId.has_value()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_owner_id"});
  }
  if (dto.farmOwnerId.has_value()) {
    columns.push_back("farm_owner_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmOwnerId), false});
  }
  if (dto.ownershipPercentage.has_value()) {
    columns.push_back("ownership_percentage");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.ownershipPercentage), false});
  }
  if (!dto.startedAt.has_value()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: started_at"});
  }
  if (dto.startedAt.has_value()) {
    columns.push_back("started_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.startedAt, false});
  }
  if (dto.endedAt.has_value()) {
    columns.push_back("ended_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.endedAt, false});
  }
  if (columns.empty()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.farm_ownership (" + columnsSql(columns) + ") VALUES (";
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
    return FarmOwnershipMutationResult::success(
        fasc::server::controllers::dto::FarmOwnershipMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipMutationResult FarmOwnershipController::update(
    const fasc::server::controllers::dto::FarmOwnershipKeyDto& key,
    const fasc::server::controllers::dto::FarmOwnershipUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (dto.farmOwnerId.has_value()) {
    columns.push_back("farm_owner_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmOwnerId), false});
  }
  if (dto.ownershipPercentage.has_value()) {
    columns.push_back("ownership_percentage");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.ownershipPercentage), false});
  }
  if (dto.startedAt.has_value()) {
    columns.push_back("started_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.startedAt, false});
  }
  if (dto.endedAt.has_value()) {
    columns.push_back("ended_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.endedAt, false});
  }
  if (columns.empty()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.farm_ownership SET ";
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
    return FarmOwnershipMutationResult::success(
        fasc::server::controllers::dto::FarmOwnershipMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipMutationResult FarmOwnershipController::erase(
    const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.farm_ownership WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return FarmOwnershipMutationResult::success(
        fasc::server::controllers::dto::FarmOwnershipMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
