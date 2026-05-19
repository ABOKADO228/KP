#include <controllers/app/FarmPlotProductionProduct.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmPlotProductionProductEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) {
  return entity.productId == key.productId &&
         entity.farmPlotId == key.farmPlotId;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmPlotProductionProductUpdateDto& dto) {
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
  }
  if (dto.productionNow.has_value()) {
    entity.productionNow = *dto.productionNow;
  }
}

} // namespace

FarmPlotProductionProductController::FarmPlotProductionProductController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotProductionProductRowsResult FarmPlotProductionProductController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmPlotProductionProductRowsDto dto;
    dto.rows = std::move(rows);
    return FarmPlotProductionProductRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductRowResult FarmPlotProductionProductController::load(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const {
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
      return FarmPlotProductionProductRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotProductionProductRowResult::success(fasc::server::controllers::dto::FarmPlotProductionProductRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductMutationResult FarmPlotProductionProductController::create(const fasc::server::controllers::dto::FarmPlotProductionProductCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.productId.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
    hasWritableValues = true;
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    entity.farmPlotId = *dto.farmPlotId;
    hasWritableValues = true;
  }
  if (!dto.quantity.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
    hasWritableValues = true;
  }
  if (!dto.productionNow.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: production_now"});
  }
  if (dto.productionNow.has_value()) {
    entity.productionNow = *dto.productionNow;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmPlotProductionProductMutationResult::success(fasc::server::controllers::dto::FarmPlotProductionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductMutationResult FarmPlotProductionProductController::update(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotProductionProductUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.quantity.has_value()) {
    hasWritableValues = true;
  }
  if (dto.productionNow.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotProductionProductMutationResult::failure(
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
    return FarmPlotProductionProductMutationResult::success(fasc::server::controllers::dto::FarmPlotProductionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductMutationResult FarmPlotProductionProductController::erase(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const {
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
    return FarmPlotProductionProductMutationResult::success(fasc::server::controllers::dto::FarmPlotProductionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
