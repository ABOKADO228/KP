#include <controllers/app/AssociationFarms.hpp>

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

fasc::server::persistence::AssociationFarmsEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::AssociationFarmsEntity entity;
  entity.farmId = std::stoi(requireColumn(row, "farm_id"));
  entity.associationId = std::stoi(requireColumn(row, "association_id"));
  entity.joinDate = requireColumn(row, "join_date");
  if (const auto value = optionalColumn(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "notes")) {
    entity.notes = *value;
  } else {
    entity.notes.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.farmId), false});
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.associationId), false});
  return values;
}

} // namespace

AssociationFarmsController::AssociationFarmsController(fasc::server::database::Database& db) : db_(db) {}

AssociationFarmsRowsResult AssociationFarmsController::list() const {
  static const std::vector<std::string> columns{"farm_id", "association_id", "join_date", "status", "notes"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.association_farms";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::AssociationFarmsRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return AssociationFarmsRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationFarmsRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsRowResult AssociationFarmsController::load(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
  static const std::vector<std::string> columns{"farm_id", "association_id", "join_date", "status", "notes"};
  static const std::vector<std::string> keys{"farm_id", "association_id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.association_farms WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return AssociationFarmsRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationFarmsRowResult::success(
        fasc::server::controllers::dto::AssociationFarmsRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return AssociationFarmsRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::create(
    const fasc::server::controllers::dto::AssociationFarmsCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.farmId.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (!dto.associationId.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (!dto.joinDate.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: join_date"});
  }
  if (dto.joinDate.has_value()) {
    columns.push_back("join_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.joinDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.association_farms (" + columnsSql(columns) + ") VALUES (";
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
    return AssociationFarmsMutationResult::success(
        fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::update(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key,
    const fasc::server::controllers::dto::AssociationFarmsUpdateDto& dto) const {
  static const std::vector<std::string> keys{"farm_id", "association_id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.joinDate.has_value()) {
    columns.push_back("join_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.joinDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.association_farms SET ";
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
    return AssociationFarmsMutationResult::success(
        fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::erase(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
  static const std::vector<std::string> keys{"farm_id", "association_id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.association_farms WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return AssociationFarmsMutationResult::success(
        fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
