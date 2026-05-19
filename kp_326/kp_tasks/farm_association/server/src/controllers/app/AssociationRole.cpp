#include <controllers/app/AssociationRole.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::AssociationRoleEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::AssociationRoleKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::AssociationRoleUpdateDto& dto) {
  if (dto.code.has_value()) {
    entity.code = *dto.code;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.description.has_value()) {
    entity.description = *dto.description;
  }
}

} // namespace

AssociationRoleController::AssociationRoleController(fasc::server::database::Database& db) : db_(db) {}

AssociationRoleRowsResult AssociationRoleController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::AssociationRoleRowsDto dto;
    dto.rows = std::move(rows);
    return AssociationRoleRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationRoleRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleRowResult AssociationRoleController::load(const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const {
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
      return AssociationRoleRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationRoleRowResult::success(fasc::server::controllers::dto::AssociationRoleRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return AssociationRoleRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleMutationResult AssociationRoleController::create(const fasc::server::controllers::dto::AssociationRoleCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.id.has_value()) {
    return AssociationRoleMutationResult::failure(
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
  if (dto.description.has_value()) {
    entity.description = *dto.description;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return AssociationRoleMutationResult::success(fasc::server::controllers::dto::AssociationRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleMutationResult AssociationRoleController::update(
    const fasc::server::controllers::dto::AssociationRoleKeyDto& key,
    const fasc::server::controllers::dto::AssociationRoleUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.code.has_value()) {
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.description.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return AssociationRoleMutationResult::failure(
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
    return AssociationRoleMutationResult::success(fasc::server::controllers::dto::AssociationRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleMutationResult AssociationRoleController::erase(const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const {
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
    return AssociationRoleMutationResult::success(fasc::server::controllers::dto::AssociationRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
