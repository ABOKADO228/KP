#include <controllers/app/FarmPlotType.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmPlotTypeEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmPlotTypeUpdateDto& dto) {
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.description.has_value()) {
    entity.description = *dto.description;
  }
  if (dto.farmPlotLevel.has_value()) {
    entity.farmPlotLevel = *dto.farmPlotLevel;
  }
  if (dto.parentId.has_value()) {
    entity.parentId = *dto.parentId;
  }
}

} // namespace

FarmPlotTypeController::FarmPlotTypeController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotTypeRowsResult FarmPlotTypeController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmPlotTypeRowsDto dto;
    dto.rows = std::move(rows);
    return FarmPlotTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeRowResult FarmPlotTypeController::load(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const {
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
      return FarmPlotTypeRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotTypeRowResult::success(fasc::server::controllers::dto::FarmPlotTypeRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeMutationResult FarmPlotTypeController::create(const fasc::server::controllers::dto::FarmPlotTypeCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.id.has_value()) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    entity.id = *dto.id;
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
    hasWritableValues = true;
  }
  if (dto.description.has_value()) {
    entity.description = *dto.description;
    hasWritableValues = true;
  }
  if (!dto.farmPlotLevel.has_value()) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_level"});
  }
  if (dto.farmPlotLevel.has_value()) {
    entity.farmPlotLevel = *dto.farmPlotLevel;
    hasWritableValues = true;
  }
  if (dto.parentId.has_value()) {
    entity.parentId = *dto.parentId;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmPlotTypeMutationResult::success(fasc::server::controllers::dto::FarmPlotTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeMutationResult FarmPlotTypeController::update(
    const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotTypeUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.description.has_value()) {
    hasWritableValues = true;
  }
  if (dto.farmPlotLevel.has_value()) {
    hasWritableValues = true;
  }
  if (dto.parentId.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotTypeMutationResult::failure(
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
    return FarmPlotTypeMutationResult::success(fasc::server::controllers::dto::FarmPlotTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeMutationResult FarmPlotTypeController::erase(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const {
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
    return FarmPlotTypeMutationResult::success(fasc::server::controllers::dto::FarmPlotTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
