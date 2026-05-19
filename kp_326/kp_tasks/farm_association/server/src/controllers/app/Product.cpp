#include <controllers/app/Product.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::ProductEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::ProductKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::ProductUpdateDto& dto) {
  if (dto.typeId.has_value()) {
    entity.typeId = *dto.typeId;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
  if (dto.unitId.has_value()) {
    entity.unitId = *dto.unitId;
  }
}

} // namespace

ProductController::ProductController(fasc::server::database::Database& db) : db_(db) {}

ProductRowsResult ProductController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::ProductRowsDto dto;
    dto.rows = std::move(rows);
    return ProductRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ProductRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductRowResult ProductController::load(const fasc::server::controllers::dto::ProductKeyDto& key) const {
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
      return ProductRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ProductRowResult::success(fasc::server::controllers::dto::ProductRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return ProductRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::create(const fasc::server::controllers::dto::ProductCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.typeId.has_value()) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: type_id"});
  }
  if (dto.typeId.has_value()) {
    entity.typeId = *dto.typeId;
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
    hasWritableValues = true;
  }
  if (dto.unitId.has_value()) {
    entity.unitId = *dto.unitId;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return ProductMutationResult::success(fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::update(
    const fasc::server::controllers::dto::ProductKeyDto& key,
    const fasc::server::controllers::dto::ProductUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.typeId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (dto.unitId.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return ProductMutationResult::failure(
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
    return ProductMutationResult::success(fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::erase(const fasc::server::controllers::dto::ProductKeyDto& key) const {
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
    return ProductMutationResult::success(fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
