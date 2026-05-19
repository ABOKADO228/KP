#include <controllers/app/EmployeePlotAssignment.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::EmployeePlotAssignmentEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::EmployeePlotAssignmentUpdateDto& dto) {
  if (dto.farmEmployeeId.has_value()) {
    entity.farmEmployeeId = *dto.farmEmployeeId;
  }
  if (dto.farmPlotId.has_value()) {
    entity.farmPlotId = *dto.farmPlotId;
  }
  if (dto.assignmentType.has_value()) {
    entity.assignmentType = *dto.assignmentType;
  }
  if (dto.assignedAt.has_value()) {
    entity.assignedAt = *dto.assignedAt;
  }
  if (dto.unassignedAt.has_value()) {
    entity.unassignedAt = *dto.unassignedAt;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
  }
}

} // namespace

EmployeePlotAssignmentController::EmployeePlotAssignmentController(fasc::server::database::Database& db) : db_(db) {}

EmployeePlotAssignmentRowsResult EmployeePlotAssignmentController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::EmployeePlotAssignmentRowsDto dto;
    dto.rows = std::move(rows);
    return EmployeePlotAssignmentRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentRowResult EmployeePlotAssignmentController::load(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const {
  try {
    auto row = db_.invokeTransactionally([&]() -> std::optional<Entity> {
      std::vector<Entity> rows = db_.selectEntities<Entity>();
      const auto iterator = std::find_if(rows.begin(), rows.end(), [&](const Entity& entity) {
        return matchesKey(entity, key);
      });
      if (iterator == rows.end()) {
        return std::nullopt;
      }
      return *iterator;
    });
    if (!row.has_value()) {
      return EmployeePlotAssignmentRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return EmployeePlotAssignmentRowResult::success(fasc::server::controllers::dto::EmployeePlotAssignmentRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentMutationResult EmployeePlotAssignmentController::create(const fasc::server::controllers::dto::EmployeePlotAssignmentCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.farmEmployeeId.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_employee_id"});
  }
  if (dto.farmEmployeeId.has_value()) {
    entity.farmEmployeeId = *dto.farmEmployeeId;
    hasWritableValues = true;
  }
  if (!dto.farmPlotId.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    entity.farmPlotId = *dto.farmPlotId;
    hasWritableValues = true;
  }
  if (dto.assignmentType.has_value()) {
    entity.assignmentType = *dto.assignmentType;
    hasWritableValues = true;
  }
  if (!dto.assignedAt.has_value()) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: assigned_at"});
  }
  if (dto.assignedAt.has_value()) {
    entity.assignedAt = *dto.assignedAt;
    hasWritableValues = true;
  }
  if (dto.unassignedAt.has_value()) {
    entity.unassignedAt = *dto.unassignedAt;
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return EmployeePlotAssignmentMutationResult::success(fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentMutationResult EmployeePlotAssignmentController::update(
    const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key,
    const fasc::server::controllers::dto::EmployeePlotAssignmentUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.farmEmployeeId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.farmPlotId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.assignmentType.has_value()) {
    hasWritableValues = true;
  }
  if (dto.assignedAt.has_value()) {
    hasWritableValues = true;
  }
  if (dto.unassignedAt.has_value()) {
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      unsigned long long count{};
      std::vector<Entity> rows = db_.selectEntities<Entity>();
      for (Entity& entity : rows) {
        if (matchesKey(entity, key)) {
          applyUpdateDto(entity, dto);
          db_.updateEntity(entity);
          ++count;
        }
      }
      return count;
    });
    return EmployeePlotAssignmentMutationResult::success(fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmployeePlotAssignmentMutationResult EmployeePlotAssignmentController::erase(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const {
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      unsigned long long count{};
      std::vector<Entity> rows = db_.selectEntities<Entity>();
      for (Entity& entity : rows) {
        if (matchesKey(entity, key)) {
          db_.eraseEntity(entity);
          ++count;
        }
      }
      return count;
    });
    return EmployeePlotAssignmentMutationResult::success(fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmployeePlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
