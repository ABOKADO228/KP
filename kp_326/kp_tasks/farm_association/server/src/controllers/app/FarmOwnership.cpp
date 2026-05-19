#include <controllers/app/FarmOwnership.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmOwnershipEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmOwnershipUpdateDto& dto) {
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
  }
  if (dto.farmOwnerId.has_value()) {
    entity.farmOwnerId = *dto.farmOwnerId;
  }
  if (dto.ownershipPercentage.has_value()) {
    entity.ownershipPercentage = *dto.ownershipPercentage;
  }
  if (dto.startedAt.has_value()) {
    entity.startedAt = *dto.startedAt;
  }
  if (dto.endedAt.has_value()) {
    entity.endedAt = *dto.endedAt;
  }
}

} // namespace

FarmOwnershipController::FarmOwnershipController(fasc::server::database::Database& db) : db_(db) {}

FarmOwnershipRowsResult FarmOwnershipController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmOwnershipRowsDto dto;
    dto.rows = std::move(rows);
    return FarmOwnershipRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmOwnershipRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipRowResult FarmOwnershipController::load(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const {
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
      return FarmOwnershipRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmOwnershipRowResult::success(fasc::server::controllers::dto::FarmOwnershipRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmOwnershipRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipMutationResult FarmOwnershipController::create(const fasc::server::controllers::dto::FarmOwnershipCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.farmId.has_value()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
    hasWritableValues = true;
  }
  if (!dto.farmOwnerId.has_value()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_owner_id"});
  }
  if (dto.farmOwnerId.has_value()) {
    entity.farmOwnerId = *dto.farmOwnerId;
    hasWritableValues = true;
  }
  if (dto.ownershipPercentage.has_value()) {
    entity.ownershipPercentage = *dto.ownershipPercentage;
    hasWritableValues = true;
  }
  if (!dto.startedAt.has_value()) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: started_at"});
  }
  if (dto.startedAt.has_value()) {
    entity.startedAt = *dto.startedAt;
    hasWritableValues = true;
  }
  if (dto.endedAt.has_value()) {
    entity.endedAt = *dto.endedAt;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmOwnershipMutationResult::success(fasc::server::controllers::dto::FarmOwnershipMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipMutationResult FarmOwnershipController::update(
    const fasc::server::controllers::dto::FarmOwnershipKeyDto& key,
    const fasc::server::controllers::dto::FarmOwnershipUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.farmId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.farmOwnerId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.ownershipPercentage.has_value()) {
    hasWritableValues = true;
  }
  if (dto.startedAt.has_value()) {
    hasWritableValues = true;
  }
  if (dto.endedAt.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmOwnershipMutationResult::failure(
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
    return FarmOwnershipMutationResult::success(fasc::server::controllers::dto::FarmOwnershipMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnershipMutationResult FarmOwnershipController::erase(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const {
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
    return FarmOwnershipMutationResult::success(fasc::server::controllers::dto::FarmOwnershipMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnershipMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
