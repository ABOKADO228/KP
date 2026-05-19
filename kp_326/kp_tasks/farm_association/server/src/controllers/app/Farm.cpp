#include <controllers/app/Farm.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmUpdateDto& dto) {
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.legalName.has_value()) {
    entity.legalName = *dto.legalName;
  }
  if (dto.legalAddress.has_value()) {
    entity.legalAddress = *dto.legalAddress;
  }
  if (dto.actualAddress.has_value()) {
    entity.actualAddress = *dto.actualAddress;
  }
  if (dto.inn.has_value()) {
    entity.inn = *dto.inn;
  }
  if (dto.ogrn.has_value()) {
    entity.ogrn = *dto.ogrn;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.farmType.has_value()) {
    entity.farmType = *dto.farmType;
  }
}

} // namespace

FarmController::FarmController(fasc::server::database::Database& db) : db_(db) {}

FarmRowsResult FarmController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmRowsDto dto;
    dto.rows = std::move(rows);
    return FarmRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRowResult FarmController::load(const fasc::server::controllers::dto::FarmKeyDto& key) const {
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
      return FarmRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmRowResult::success(fasc::server::controllers::dto::FarmRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmMutationResult FarmController::create(const fasc::server::controllers::dto::FarmCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (dto.name.has_value()) {
    entity.name = *dto.name;
    hasWritableValues = true;
  }
  if (dto.legalName.has_value()) {
    entity.legalName = *dto.legalName;
    hasWritableValues = true;
  }
  if (dto.legalAddress.has_value()) {
    entity.legalAddress = *dto.legalAddress;
    hasWritableValues = true;
  }
  if (dto.actualAddress.has_value()) {
    entity.actualAddress = *dto.actualAddress;
    hasWritableValues = true;
  }
  if (dto.inn.has_value()) {
    entity.inn = *dto.inn;
    hasWritableValues = true;
  }
  if (dto.ogrn.has_value()) {
    entity.ogrn = *dto.ogrn;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (dto.farmType.has_value()) {
    entity.farmType = *dto.farmType;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmMutationResult::success(fasc::server::controllers::dto::FarmMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmMutationResult FarmController::update(
    const fasc::server::controllers::dto::FarmKeyDto& key,
    const fasc::server::controllers::dto::FarmUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.legalName.has_value()) {
    hasWritableValues = true;
  }
  if (dto.legalAddress.has_value()) {
    hasWritableValues = true;
  }
  if (dto.actualAddress.has_value()) {
    hasWritableValues = true;
  }
  if (dto.inn.has_value()) {
    hasWritableValues = true;
  }
  if (dto.ogrn.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.farmType.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmMutationResult::failure(
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
    return FarmMutationResult::success(fasc::server::controllers::dto::FarmMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmMutationResult FarmController::erase(const fasc::server::controllers::dto::FarmKeyDto& key) const {
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
    return FarmMutationResult::success(fasc::server::controllers::dto::FarmMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
