#include <controllers/app/EmployeePlotAssignment.hpp>

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


fasc::server::persistence::EmployeePlotAssignmentEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::EmployeePlotAssignmentEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  entity.farmEmployeeId = fasc::server::database::requireColumn<std::uint64_t>(row, "farm_employee_id");
  entity.farmPlotId = fasc::server::database::requireColumn<std::uint64_t>(row, "farm_plot_id");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::EmployeePlotAssignmentType>(row, "assignment_type")) {
    entity.assignmentType = *value;
  } else {
    entity.assignmentType.reset();
  }
  entity.assignedAt = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "assigned_at");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::Date>(row, "unassigned_at")) {
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
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

EmployeePlotAssignmentController::EmployeePlotAssignmentController(fasc::server::database::Database& db) : db_(db) {}

EmployeePlotAssignmentRowsResult EmployeePlotAssignmentController::list() const {
  static const std::vector<std::string> columns{"id", "farm_employee_id", "farm_plot_id", "assignment_type", "assigned_at", "unassigned_at", "notes"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.employee_plot_assignment", columns);
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
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.employee_plot_assignment", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return EmployeePlotAssignmentRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return EmployeePlotAssignmentRowResult::success(
        fasc::server::controllers::dto::EmployeePlotAssignmentRowDto{rowToEntity(*row)});
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmEmployeeId));
  }
  if (!dto.farmPlotId.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmPlotId));
  }
  if (dto.assignmentType.has_value()) {
    columns.push_back("assignment_type");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.assignmentType));
  }
  if (!dto.assignedAt.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: assigned_at"});
  }
  if (dto.assignedAt.has_value()) {
    columns.push_back("assigned_at");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.assignedAt));
  }
  if (dto.unassignedAt.has_value()) {
    columns.push_back("unassigned_at");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.unassignedAt));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.employee_plot_assignment", columns, values);
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmEmployeeId));
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmPlotId));
  }
  if (dto.assignmentType.has_value()) {
    columns.push_back("assignment_type");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.assignmentType));
  }
  if (dto.assignedAt.has_value()) {
    columns.push_back("assigned_at");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.assignedAt));
  }
  if (dto.unassignedAt.has_value()) {
    columns.push_back("unassigned_at");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.unassignedAt));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.employee_plot_assignment", columns, values, keys, keyValues(key));
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
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.employee_plot_assignment", keys, keyValues(key));
    });
    return EmployeePlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
