#include <controllers/app/FarmPlotAssignment.hpp>

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

fasc::server::persistence::FarmPlotAssignmentEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmPlotAssignmentEntity entity;
  entity.farmId = std::stoi(requireColumn(row, "farm_id"));
  entity.farmPlotId = std::stoi(requireColumn(row, "farm_plot_id"));
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
    const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.farmId), false});
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.farmPlotId), false});
  return values;
}

} // namespace

FarmPlotAssignmentController::FarmPlotAssignmentController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotAssignmentRowsResult FarmPlotAssignmentController::list() const {
  static const std::vector<std::string> columns{"farm_id", "farm_plot_id", "status", "notes"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_plot_assignment";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::FarmPlotAssignmentRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmPlotAssignmentRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentRowResult FarmPlotAssignmentController::load(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const {
  static const std::vector<std::string> columns{"farm_id", "farm_plot_id", "status", "notes"};
  static const std::vector<std::string> keys{"farm_id", "farm_plot_id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_plot_assignment WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return FarmPlotAssignmentRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotAssignmentRowResult::success(
        fasc::server::controllers::dto::FarmPlotAssignmentRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentMutationResult FarmPlotAssignmentController::create(
    const fasc::server::controllers::dto::FarmPlotAssignmentCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.farmId.has_value()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotId), false});
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
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.farm_plot_assignment (" + columnsSql(columns) + ") VALUES (";
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
    return FarmPlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::FarmPlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentMutationResult FarmPlotAssignmentController::update(
    const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotAssignmentUpdateDto& dto) const {
  static const std::vector<std::string> keys{"farm_id", "farm_plot_id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.farm_plot_assignment SET ";
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
    return FarmPlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::FarmPlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentMutationResult FarmPlotAssignmentController::erase(
    const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const {
  static const std::vector<std::string> keys{"farm_id", "farm_plot_id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.farm_plot_assignment WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return FarmPlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::FarmPlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
