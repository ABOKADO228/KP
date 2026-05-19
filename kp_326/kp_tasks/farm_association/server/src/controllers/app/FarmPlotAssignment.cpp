#include <controllers/app/FarmPlotAssignment.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmPlotAssignmentEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) {
  return entity.farmId == key.farmId &&
         entity.farmPlotId == key.farmPlotId;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmPlotAssignmentUpdateDto& dto) {
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
  }
}

} // namespace

FarmPlotAssignmentController::FarmPlotAssignmentController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotAssignmentRowsResult FarmPlotAssignmentController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmPlotAssignmentRowsDto dto;
    dto.rows = std::move(rows);
    return FarmPlotAssignmentRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentRowResult FarmPlotAssignmentController::load(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const {
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
      return FarmPlotAssignmentRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotAssignmentRowResult::success(fasc::server::controllers::dto::FarmPlotAssignmentRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentMutationResult FarmPlotAssignmentController::create(const fasc::server::controllers::dto::FarmPlotAssignmentCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.farmId.has_value()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
    hasWritableValues = true;
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    entity.farmPlotId = *dto.farmPlotId;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmPlotAssignmentMutationResult::success(fasc::server::controllers::dto::FarmPlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentMutationResult FarmPlotAssignmentController::update(
    const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotAssignmentUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotAssignmentMutationResult::failure(
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
    return FarmPlotAssignmentMutationResult::success(fasc::server::controllers::dto::FarmPlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotAssignmentMutationResult FarmPlotAssignmentController::erase(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const {
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
    return FarmPlotAssignmentMutationResult::success(fasc::server::controllers::dto::FarmPlotAssignmentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotAssignmentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
