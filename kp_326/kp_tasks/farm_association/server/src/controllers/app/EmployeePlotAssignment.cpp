#include <controllers/app/EmployeePlotAssignment.hpp>

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

fasc::server::persistence::EmployeePlotAssignmentEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::EmployeePlotAssignmentEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.farmEmployeeId = std::stoi(requireColumn(row, "farm_employee_id"));
  entity.farmPlotId = std::stoi(requireColumn(row, "farm_plot_id"));
  if (const auto value = optionalColumn(row, "assignment_type")) {
    entity.assignmentType = *value;
  } else {
    entity.assignmentType.reset();
  }
  entity.assignedAt = requireColumn(row, "assigned_at");
  if (const auto value = optionalColumn(row, "unassigned_at")) {
    entity.unassignedAt = *value;
  } else {
    entity.unassignedAt.reset();
  }
  if (const auto value = optionalColumn(row, "notes")) {
    entity.notes = *value;
  } else {
    entity.notes.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

EmployeePlotAssignmentController::EmployeePlotAssignmentController(fasc::server::database::Database& db) : db_(db) {}

EmployeePlotAssignmentRowsResult EmployeePlotAssignmentController::list() const {
  static const std::vector<std::string> columns{"id", "farm_employee_id", "farm_plot_id", "assignment_type", "assigned_at", "unassigned_at", "notes"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.employee_plot_assignment";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::EmployeePlotAssignmentRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return EmployeePlotAssignmentRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentRowResult EmployeePlotAssignmentController::load(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "farm_employee_id", "farm_plot_id", "assignment_type", "assigned_at", "unassigned_at", "notes"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.employee_plot_assignment WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return EmployeePlotAssignmentRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return EmployeePlotAssignmentRowResult::success(
        fasc::server::controllers::dto::EmployeePlotAssignmentRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentMutationResult EmployeePlotAssignmentController::create(
    const fasc::server::controllers::dto::EmployeePlotAssignmentCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.farmEmployeeId.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_employee_id"});
  }
  if (dto.farmEmployeeId.has_value()) {
    columns.push_back("farm_employee_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmEmployeeId), false});
  }
  if (!dto.farmPlotId.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotId), false});
  }
  if (dto.assignmentType.has_value()) {
    columns.push_back("assignment_type");
    values.push_back(fasc::server::database::SqlParameter{*dto.assignmentType, false});
  }
  if (!dto.assignedAt.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: assigned_at"});
  }
  if (dto.assignedAt.has_value()) {
    columns.push_back("assigned_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.assignedAt, false});
  }
  if (dto.unassignedAt.has_value()) {
    columns.push_back("unassigned_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.unassignedAt, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.employee_plot_assignment (" + columnsSql(columns) + ") VALUES (";
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
    return EmployeePlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentMutationResult EmployeePlotAssignmentController::update(
    const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key,
    const fasc::server::controllers::dto::EmployeePlotAssignmentUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.farmEmployeeId.has_value()) {
    columns.push_back("farm_employee_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmEmployeeId), false});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotId), false});
  }
  if (dto.assignmentType.has_value()) {
    columns.push_back("assignment_type");
    values.push_back(fasc::server::database::SqlParameter{*dto.assignmentType, false});
  }
  if (dto.assignedAt.has_value()) {
    columns.push_back("assigned_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.assignedAt, false});
  }
  if (dto.unassignedAt.has_value()) {
    columns.push_back("unassigned_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.unassignedAt, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.employee_plot_assignment SET ";
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
    return EmployeePlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentMutationResult EmployeePlotAssignmentController::erase(
    const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.employee_plot_assignment WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return EmployeePlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
