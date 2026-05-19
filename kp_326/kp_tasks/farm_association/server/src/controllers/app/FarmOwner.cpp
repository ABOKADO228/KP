#include <controllers/app/FarmOwner.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmOwnerEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmOwnerKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmOwnerUpdateDto& dto) {
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.rating.has_value()) {
    entity.rating = *dto.rating;
  }
}

} // namespace

FarmOwnerController::FarmOwnerController(fasc::server::database::Database& db) : db_(db) {}

FarmOwnerRowsResult FarmOwnerController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmOwnerRowsDto dto;
    dto.rows = std::move(rows);
    return FarmOwnerRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmOwnerRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerRowResult FarmOwnerController::load(const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const {
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
      return FarmOwnerRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmOwnerRowResult::success(fasc::server::controllers::dto::FarmOwnerRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmOwnerRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerMutationResult FarmOwnerController::create(const fasc::server::controllers::dto::FarmOwnerCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.personId.has_value()) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (dto.rating.has_value()) {
    entity.rating = *dto.rating;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmOwnerMutationResult::success(fasc::server::controllers::dto::FarmOwnerMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerMutationResult FarmOwnerController::update(
    const fasc::server::controllers::dto::FarmOwnerKeyDto& key,
    const fasc::server::controllers::dto::FarmOwnerUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.personId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.rating.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmOwnerMutationResult::failure(
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
    return FarmOwnerMutationResult::success(fasc::server::controllers::dto::FarmOwnerMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerMutationResult FarmOwnerController::erase(const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const {
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
    return FarmOwnerMutationResult::success(fasc::server::controllers::dto::FarmOwnerMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
