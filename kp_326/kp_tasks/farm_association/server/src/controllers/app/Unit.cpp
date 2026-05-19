#include <controllers/app/Unit.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::UnitEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::UnitKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::UnitUpdateDto& dto) {
  if (dto.code.has_value()) {
    entity.code = *dto.code;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
}

} // namespace

UnitController::UnitController(fasc::server::database::Database& db) : db_(db) {}

UnitRowsResult UnitController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::UnitRowsDto dto;
    dto.rows = std::move(rows);
    return UnitRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return UnitRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitRowResult UnitController::load(const fasc::server::controllers::dto::UnitKeyDto& key) const {
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
      return UnitRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return UnitRowResult::success(fasc::server::controllers::dto::UnitRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return UnitRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitMutationResult UnitController::create(const fasc::server::controllers::dto::UnitCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.id.has_value()) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    entity.id = *dto.id;
    hasWritableValues = true;
  }
  if (dto.code.has_value()) {
    entity.code = *dto.code;
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return UnitMutationResult::success(fasc::server::controllers::dto::UnitMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitMutationResult UnitController::update(
    const fasc::server::controllers::dto::UnitKeyDto& key,
    const fasc::server::controllers::dto::UnitUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.code.has_value()) {
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return UnitMutationResult::failure(
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
    return UnitMutationResult::success(fasc::server::controllers::dto::UnitMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitMutationResult UnitController::erase(const fasc::server::controllers::dto::UnitKeyDto& key) const {
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
    return UnitMutationResult::success(fasc::server::controllers::dto::UnitMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
