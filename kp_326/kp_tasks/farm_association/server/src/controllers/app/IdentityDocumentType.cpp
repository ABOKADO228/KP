#include <controllers/app/IdentityDocumentType.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::IdentityDocumentTypeEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::IdentityDocumentTypeUpdateDto& dto) {
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

IdentityDocumentTypeController::IdentityDocumentTypeController(fasc::server::database::Database& db) : db_(db) {}

IdentityDocumentTypeRowsResult IdentityDocumentTypeController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::IdentityDocumentTypeRowsDto dto;
    dto.rows = std::move(rows);
    return IdentityDocumentTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeRowResult IdentityDocumentTypeController::load(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
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
      return IdentityDocumentTypeRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return IdentityDocumentTypeRowResult::success(fasc::server::controllers::dto::IdentityDocumentTypeRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::create(const fasc::server::controllers::dto::IdentityDocumentTypeCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.id.has_value()) {
    return IdentityDocumentTypeMutationResult::failure(
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
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return IdentityDocumentTypeMutationResult::success(fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::update(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key,
    const fasc::server::controllers::dto::IdentityDocumentTypeUpdateDto& dto) const {
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
    return IdentityDocumentTypeMutationResult::failure(
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
    return IdentityDocumentTypeMutationResult::success(fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::erase(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
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
    return IdentityDocumentTypeMutationResult::success(fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
