#include <controllers/app/Supplier.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::SupplierEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::SupplierKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::SupplierUpdateDto& dto) {
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

SupplierController::SupplierController(fasc::server::database::Database& db) : db_(db) {}

SupplierRowsResult SupplierController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::SupplierRowsDto dto;
    dto.rows = std::move(rows);
    return SupplierRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return SupplierRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierRowResult SupplierController::load(const fasc::server::controllers::dto::SupplierKeyDto& key) const {
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
      return SupplierRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return SupplierRowResult::success(fasc::server::controllers::dto::SupplierRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return SupplierRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierMutationResult SupplierController::create(const fasc::server::controllers::dto::SupplierCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
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
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return SupplierMutationResult::success(fasc::server::controllers::dto::SupplierMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierMutationResult SupplierController::update(
    const fasc::server::controllers::dto::SupplierKeyDto& key,
    const fasc::server::controllers::dto::SupplierUpdateDto& dto) const {
  bool hasWritableValues = false;
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
    return SupplierMutationResult::failure(
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
    return SupplierMutationResult::success(fasc::server::controllers::dto::SupplierMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierMutationResult SupplierController::erase(const fasc::server::controllers::dto::SupplierKeyDto& key) const {
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
    return SupplierMutationResult::success(fasc::server::controllers::dto::SupplierMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
