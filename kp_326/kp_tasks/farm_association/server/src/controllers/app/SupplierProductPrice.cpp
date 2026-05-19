#include <controllers/app/SupplierProductPrice.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::SupplierProductPriceEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::SupplierProductPriceUpdateDto& dto) {
  if (dto.supplierId.has_value()) {
    entity.supplierId = *dto.supplierId;
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
  }
  if (dto.purchasePrice.has_value()) {
    entity.purchasePrice = *dto.purchasePrice;
  }
  if (dto.validFrom.has_value()) {
    entity.validFrom = *dto.validFrom;
  }
  if (dto.validUntil.has_value()) {
    entity.validUntil = *dto.validUntil;
  }
}

} // namespace

SupplierProductPriceController::SupplierProductPriceController(fasc::server::database::Database& db) : db_(db) {}

SupplierProductPriceRowsResult SupplierProductPriceController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::SupplierProductPriceRowsDto dto;
    dto.rows = std::move(rows);
    return SupplierProductPriceRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return SupplierProductPriceRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceRowResult SupplierProductPriceController::load(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const {
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
      return SupplierProductPriceRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return SupplierProductPriceRowResult::success(fasc::server::controllers::dto::SupplierProductPriceRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return SupplierProductPriceRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceMutationResult SupplierProductPriceController::create(const fasc::server::controllers::dto::SupplierProductPriceCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.supplierId.has_value()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: supplier_id"});
  }
  if (dto.supplierId.has_value()) {
    entity.supplierId = *dto.supplierId;
    hasWritableValues = true;
  }
  if (!dto.productId.has_value()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
    hasWritableValues = true;
  }
  if (dto.purchasePrice.has_value()) {
    entity.purchasePrice = *dto.purchasePrice;
    hasWritableValues = true;
  }
  if (!dto.validFrom.has_value()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: valid_from"});
  }
  if (dto.validFrom.has_value()) {
    entity.validFrom = *dto.validFrom;
    hasWritableValues = true;
  }
  if (dto.validUntil.has_value()) {
    entity.validUntil = *dto.validUntil;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return SupplierProductPriceMutationResult::success(fasc::server::controllers::dto::SupplierProductPriceMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceMutationResult SupplierProductPriceController::update(
    const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key,
    const fasc::server::controllers::dto::SupplierProductPriceUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.supplierId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.productId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.purchasePrice.has_value()) {
    hasWritableValues = true;
  }
  if (dto.validFrom.has_value()) {
    hasWritableValues = true;
  }
  if (dto.validUntil.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return SupplierProductPriceMutationResult::failure(
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
    return SupplierProductPriceMutationResult::success(fasc::server::controllers::dto::SupplierProductPriceMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceMutationResult SupplierProductPriceController::erase(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const {
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
    return SupplierProductPriceMutationResult::success(fasc::server::controllers::dto::SupplierProductPriceMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
