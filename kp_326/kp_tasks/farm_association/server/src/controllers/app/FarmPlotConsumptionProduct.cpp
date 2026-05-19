#include <controllers/app/FarmPlotConsumptionProduct.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmPlotConsumptionProductEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) {
  return entity.productId == key.productId &&
         entity.farmPlotId == key.farmPlotId;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmPlotConsumptionProductUpdateDto& dto) {
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
  }
  if (dto.consumptionNow.has_value()) {
    entity.consumptionNow = *dto.consumptionNow;
  }
}

} // namespace

FarmPlotConsumptionProductController::FarmPlotConsumptionProductController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotConsumptionProductRowsResult FarmPlotConsumptionProductController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmPlotConsumptionProductRowsDto dto;
    dto.rows = std::move(rows);
    return FarmPlotConsumptionProductRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductRowResult FarmPlotConsumptionProductController::load(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const {
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
      return FarmPlotConsumptionProductRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotConsumptionProductRowResult::success(fasc::server::controllers::dto::FarmPlotConsumptionProductRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductMutationResult FarmPlotConsumptionProductController::create(const fasc::server::controllers::dto::FarmPlotConsumptionProductCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.productId.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
    hasWritableValues = true;
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    entity.farmPlotId = *dto.farmPlotId;
    hasWritableValues = true;
  }
  if (!dto.quantity.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
    hasWritableValues = true;
  }
  if (!dto.consumptionNow.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: consumption_now"});
  }
  if (dto.consumptionNow.has_value()) {
    entity.consumptionNow = *dto.consumptionNow;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmPlotConsumptionProductMutationResult::success(fasc::server::controllers::dto::FarmPlotConsumptionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductMutationResult FarmPlotConsumptionProductController::update(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotConsumptionProductUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.quantity.has_value()) {
    hasWritableValues = true;
  }
  if (dto.consumptionNow.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmPlotConsumptionProductMutationResult::failure(
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
    return FarmPlotConsumptionProductMutationResult::success(fasc::server::controllers::dto::FarmPlotConsumptionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductMutationResult FarmPlotConsumptionProductController::erase(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const {
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
    return FarmPlotConsumptionProductMutationResult::success(fasc::server::controllers::dto::FarmPlotConsumptionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
