#include <controllers/app/FarmRole.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmRoleEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmRoleKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmRoleUpdateDto& dto) {
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.description.has_value()) {
    entity.description = *dto.description;
  }
}

} // namespace

FarmRoleController::FarmRoleController(fasc::server::database::Database& db) : db_(db) {}

FarmRoleRowsResult FarmRoleController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmRoleRowsDto dto;
    dto.rows = std::move(rows);
    return FarmRoleRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmRoleRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleRowResult FarmRoleController::load(const fasc::server::controllers::dto::FarmRoleKeyDto& key) const {
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
      return FarmRoleRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmRoleRowResult::success(fasc::server::controllers::dto::FarmRoleRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmRoleRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleMutationResult FarmRoleController::create(const fasc::server::controllers::dto::FarmRoleCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.id.has_value()) {
    return FarmRoleMutationResult::failure(
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
  if (!hasWritableValues) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmRoleMutationResult::success(fasc::server::controllers::dto::FarmRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleMutationResult FarmRoleController::update(
    const fasc::server::controllers::dto::FarmRoleKeyDto& key,
    const fasc::server::controllers::dto::FarmRoleUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.description.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmRoleMutationResult::failure(
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
    return FarmRoleMutationResult::success(fasc::server::controllers::dto::FarmRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleMutationResult FarmRoleController::erase(const fasc::server::controllers::dto::FarmRoleKeyDto& key) const {
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
    return FarmRoleMutationResult::success(fasc::server::controllers::dto::FarmRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
