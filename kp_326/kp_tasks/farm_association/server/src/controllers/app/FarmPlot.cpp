#include <controllers/app/FarmPlot.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmPlotEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmPlotKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmPlotUpdateDto& dto) {
  if (dto.farmPlotTypeId.has_value()) {
    entity.farmPlotTypeId = *dto.farmPlotTypeId;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.area.has_value()) {
    entity.area = *dto.area;
  }
  if (dto.location.has_value()) {
    entity.location = *dto.location;
  }
  if (dto.cadastralNumber.has_value()) {
    entity.cadastralNumber = *dto.cadastralNumber;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
}

} // namespace

FarmPlotController::FarmPlotController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotRowsResult FarmPlotController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmPlotRowsDto dto;
    dto.rows = std::move(rows);
    return FarmPlotRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotRowResult FarmPlotController::load(const fasc::server::controllers::dto::FarmPlotKeyDto& key) const {
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
      return FarmPlotRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotRowResult::success(fasc::server::controllers::dto::FarmPlotRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotMutationResult FarmPlotController::create(const fasc::server::controllers::dto::FarmPlotCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (dto.farmPlotTypeId.has_value()) {
    entity.farmPlotTypeId = *dto.farmPlotTypeId;
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
    hasWritableValues = true;
  }
  if (dto.area.has_value()) {
    entity.area = *dto.area;
    hasWritableValues = true;
  }
  if (dto.location.has_value()) {
    entity.location = *dto.location;
    hasWritableValues = true;
  }
  if (dto.cadastralNumber.has_value()) {
    entity.cadastralNumber = *dto.cadastralNumber;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmPlotMutationResult::success(fasc::server::controllers::dto::FarmPlotMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotMutationResult FarmPlotController::update(
    const fasc::server::controllers::dto::FarmPlotKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.farmPlotTypeId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.area.has_value()) {
    hasWritableValues = true;
  }
  if (dto.location.has_value()) {
    hasWritableValues = true;
  }
  if (dto.cadastralNumber.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotMutationResult::failure(
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
    return FarmPlotMutationResult::success(fasc::server::controllers::dto::FarmPlotMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotMutationResult FarmPlotController::erase(const fasc::server::controllers::dto::FarmPlotKeyDto& key) const {
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
    return FarmPlotMutationResult::success(fasc::server::controllers::dto::FarmPlotMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
