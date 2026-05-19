#include <controllers/app/ProductType.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::ProductTypeEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::ProductTypeKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::ProductTypeUpdateDto& dto) {
  if (dto.parentId.has_value()) {
    entity.parentId = *dto.parentId;
  }
  if (dto.sku.has_value()) {
    entity.sku = *dto.sku;
  }
  if (dto.productLevel.has_value()) {
    entity.productLevel = *dto.productLevel;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.description.has_value()) {
    entity.description = *dto.description;
  }
}

} // namespace

ProductTypeController::ProductTypeController(fasc::server::database::Database& db) : db_(db) {}

ProductTypeRowsResult ProductTypeController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::ProductTypeRowsDto dto;
    dto.rows = std::move(rows);
    return ProductTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ProductTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeRowResult ProductTypeController::load(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
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
      return ProductTypeRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ProductTypeRowResult::success(fasc::server::controllers::dto::ProductTypeRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return ProductTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::create(const fasc::server::controllers::dto::ProductTypeCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.id.has_value()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    entity.id = *dto.id;
    hasWritableValues = true;
  }
  if (dto.parentId.has_value()) {
    entity.parentId = *dto.parentId;
    hasWritableValues = true;
  }
  if (dto.sku.has_value()) {
    entity.sku = *dto.sku;
    hasWritableValues = true;
  }
  if (!dto.productLevel.has_value()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_level"});
  }
  if (dto.productLevel.has_value()) {
    entity.productLevel = *dto.productLevel;
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
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return ProductTypeMutationResult::success(fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::update(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key,
    const fasc::server::controllers::dto::ProductTypeUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.parentId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.sku.has_value()) {
    hasWritableValues = true;
  }
  if (dto.productLevel.has_value()) {
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.description.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return ProductTypeMutationResult::failure(
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
    return ProductTypeMutationResult::success(fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::erase(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
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
    return ProductTypeMutationResult::success(fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
