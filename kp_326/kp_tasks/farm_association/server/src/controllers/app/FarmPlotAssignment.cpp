#include <controllers/app/FarmPlotAssignment.hpp>

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


fasc::server::persistence::FarmPlotAssignmentEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmPlotAssignmentEntity entity;
  entity.farmId = fasc::server::database::requireColumn<std::uint64_t>(row, "farm_id");
  entity.farmPlotId = fasc::server::database::requireColumn<std::uint64_t>(row, "farm_plot_id");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::FarmPlotAssignmentStatus>(row, "status")) {
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
  values.push_back(fasc::server::database::makeSqlParameter(key.farmId));
  values.push_back(fasc::server::database::makeSqlParameter(key.farmPlotId));
  return values;
}

} // namespace

FarmPlotAssignmentController::FarmPlotAssignmentController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotAssignmentRowsResult FarmPlotAssignmentController::list() const {
  static const std::vector<std::string> columns{"farm_id", "farm_plot_id", "status", "notes"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_plot_assignment", columns);
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
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_plot_assignment", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmPlotAssignmentRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotAssignmentRowResult::success(
        fasc::server::controllers::dto::FarmPlotAssignmentRowDto{rowToEntity(*row)});
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmId));
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmPlotId));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_plot_assignment", columns, values);
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_plot_assignment", columns, values, keys, keyValues(key));
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
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_plot_assignment", keys, keyValues(key));
    });
    return FarmPlotAssignmentMutationResult::success(
        fasc::server::controllers::dto::FarmPlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
