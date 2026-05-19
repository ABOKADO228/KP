#include <controllers/app/AssociationFarms.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::AssociationFarmsEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) {
  return entity.farmId == key.farmId &&
         entity.associationId == key.associationId;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::AssociationFarmsUpdateDto& dto) {
  if (dto.joinDate.has_value()) {
    entity.joinDate = *dto.joinDate;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
  }
}

} // namespace

AssociationFarmsController::AssociationFarmsController(fasc::server::database::Database& db) : db_(db) {}

AssociationFarmsRowsResult AssociationFarmsController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::AssociationFarmsRowsDto dto;
    dto.rows = std::move(rows);
    return AssociationFarmsRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationFarmsRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsRowResult AssociationFarmsController::load(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
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
      return AssociationFarmsRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationFarmsRowResult::success(fasc::server::controllers::dto::AssociationFarmsRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return AssociationFarmsRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::create(const fasc::server::controllers::dto::AssociationFarmsCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.farmId.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
    hasWritableValues = true;
  }
  if (!dto.associationId.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
    hasWritableValues = true;
  }
  if (!dto.joinDate.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: join_date"});
  }
  if (dto.joinDate.has_value()) {
    entity.joinDate = *dto.joinDate;
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
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return AssociationFarmsMutationResult::success(fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::update(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key,
    const fasc::server::controllers::dto::AssociationFarmsUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.joinDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return AssociationFarmsMutationResult::failure(
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
    return AssociationFarmsMutationResult::success(fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::erase(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
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
    return AssociationFarmsMutationResult::success(fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
