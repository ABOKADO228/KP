#include <controllers/app/PersonDocument.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::PersonDocumentEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::PersonDocumentKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::PersonDocumentUpdateDto& dto) {
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
  }
  if (dto.documentTypeId.has_value()) {
    entity.documentTypeId = *dto.documentTypeId;
  }
  if (dto.documentNumber.has_value()) {
    entity.documentNumber = *dto.documentNumber;
  }
  if (dto.issuedBy.has_value()) {
    entity.issuedBy = *dto.issuedBy;
  }
  if (dto.issuedDate.has_value()) {
    entity.issuedDate = *dto.issuedDate;
  }
  if (dto.expirationDate.has_value()) {
    entity.expirationDate = *dto.expirationDate;
  }
  if (dto.isPrimary.has_value()) {
    entity.isPrimary = *dto.isPrimary;
  }
}

} // namespace

PersonDocumentController::PersonDocumentController(fasc::server::database::Database& db) : db_(db) {}

PersonDocumentRowsResult PersonDocumentController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::PersonDocumentRowsDto dto;
    dto.rows = std::move(rows);
    return PersonDocumentRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PersonDocumentRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentRowResult PersonDocumentController::load(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
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
      return PersonDocumentRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PersonDocumentRowResult::success(fasc::server::controllers::dto::PersonDocumentRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return PersonDocumentRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::create(const fasc::server::controllers::dto::PersonDocumentCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.personId.has_value()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
    hasWritableValues = true;
  }
  if (!dto.documentTypeId.has_value()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: document_type_id"});
  }
  if (dto.documentTypeId.has_value()) {
    entity.documentTypeId = *dto.documentTypeId;
    hasWritableValues = true;
  }
  if (dto.documentNumber.has_value()) {
    entity.documentNumber = *dto.documentNumber;
    hasWritableValues = true;
  }
  if (dto.issuedBy.has_value()) {
    entity.issuedBy = *dto.issuedBy;
    hasWritableValues = true;
  }
  if (dto.issuedDate.has_value()) {
    entity.issuedDate = *dto.issuedDate;
    hasWritableValues = true;
  }
  if (dto.expirationDate.has_value()) {
    entity.expirationDate = *dto.expirationDate;
    hasWritableValues = true;
  }
  if (dto.isPrimary.has_value()) {
    entity.isPrimary = *dto.isPrimary;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return PersonDocumentMutationResult::success(fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::update(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key,
    const fasc::server::controllers::dto::PersonDocumentUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.personId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.documentTypeId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.documentNumber.has_value()) {
    hasWritableValues = true;
  }
  if (dto.issuedBy.has_value()) {
    hasWritableValues = true;
  }
  if (dto.issuedDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.expirationDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.isPrimary.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PersonDocumentMutationResult::failure(
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
    return PersonDocumentMutationResult::success(fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::erase(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
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
    return PersonDocumentMutationResult::success(fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
