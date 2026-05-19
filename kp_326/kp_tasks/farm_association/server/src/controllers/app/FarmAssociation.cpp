#include <controllers/app/FarmAssociation.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmAssociationEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmAssociationKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmAssociationUpdateDto& dto) {
  if (dto.inn.has_value()) {
    entity.inn = *dto.inn;
  }
  if (dto.ogrn.has_value()) {
    entity.ogrn = *dto.ogrn;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.legalAddress.has_value()) {
    entity.legalAddress = *dto.legalAddress;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
}

} // namespace

FarmAssociationController::FarmAssociationController(fasc::server::database::Database& db) : db_(db) {}

FarmAssociationRowsResult FarmAssociationController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmAssociationRowsDto dto;
    dto.rows = std::move(rows);
    return FarmAssociationRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmAssociationRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationRowResult FarmAssociationController::load(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
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
      return FarmAssociationRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmAssociationRowResult::success(fasc::server::controllers::dto::FarmAssociationRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmAssociationRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::create(const fasc::server::controllers::dto::FarmAssociationCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (dto.inn.has_value()) {
    entity.inn = *dto.inn;
    hasWritableValues = true;
  }
  if (dto.ogrn.has_value()) {
    entity.ogrn = *dto.ogrn;
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
    hasWritableValues = true;
  }
  if (dto.legalAddress.has_value()) {
    entity.legalAddress = *dto.legalAddress;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmAssociationMutationResult::success(fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::update(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key,
    const fasc::server::controllers::dto::FarmAssociationUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.inn.has_value()) {
    hasWritableValues = true;
  }
  if (dto.ogrn.has_value()) {
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.legalAddress.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmAssociationMutationResult::failure(
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
    return FarmAssociationMutationResult::success(fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::erase(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
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
    return FarmAssociationMutationResult::success(fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
