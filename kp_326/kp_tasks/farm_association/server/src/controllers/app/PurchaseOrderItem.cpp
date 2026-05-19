#include <controllers/app/PurchaseOrderItem.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::PurchaseOrderItemEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::PurchaseOrderItemUpdateDto& dto) {
  if (dto.purchaseOrderId.has_value()) {
    entity.purchaseOrderId = *dto.purchaseOrderId;
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
  }
  if (dto.unitPrice.has_value()) {
    entity.unitPrice = *dto.unitPrice;
  }
  if (dto.vatRate.has_value()) {
    entity.vatRate = *dto.vatRate;
  }
  if (dto.currency.has_value()) {
    entity.currency = *dto.currency;
  }
}

} // namespace

PurchaseOrderItemController::PurchaseOrderItemController(fasc::server::database::Database& db) : db_(db) {}

PurchaseOrderItemRowsResult PurchaseOrderItemController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::PurchaseOrderItemRowsDto dto;
    dto.rows = std::move(rows);
    return PurchaseOrderItemRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PurchaseOrderItemRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemRowResult PurchaseOrderItemController::load(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const {
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
      return PurchaseOrderItemRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseOrderItemRowResult::success(fasc::server::controllers::dto::PurchaseOrderItemRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemMutationResult PurchaseOrderItemController::create(const fasc::server::controllers::dto::PurchaseOrderItemCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.purchaseOrderId.has_value()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: purchase_order_id"});
  }
  if (dto.purchaseOrderId.has_value()) {
    entity.purchaseOrderId = *dto.purchaseOrderId;
    hasWritableValues = true;
  }
  if (!dto.productId.has_value()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
    hasWritableValues = true;
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
    hasWritableValues = true;
  }
  if (dto.unitPrice.has_value()) {
    entity.unitPrice = *dto.unitPrice;
    hasWritableValues = true;
  }
  if (dto.vatRate.has_value()) {
    entity.vatRate = *dto.vatRate;
    hasWritableValues = true;
  }
  if (dto.currency.has_value()) {
    entity.currency = *dto.currency;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return PurchaseOrderItemMutationResult::success(fasc::server::controllers::dto::PurchaseOrderItemMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemMutationResult PurchaseOrderItemController::update(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key,
    const fasc::server::controllers::dto::PurchaseOrderItemUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.purchaseOrderId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.productId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.quantity.has_value()) {
    hasWritableValues = true;
  }
  if (dto.unitPrice.has_value()) {
    hasWritableValues = true;
  }
  if (dto.vatRate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.currency.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PurchaseOrderItemMutationResult::failure(
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
    return PurchaseOrderItemMutationResult::success(fasc::server::controllers::dto::PurchaseOrderItemMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemMutationResult PurchaseOrderItemController::erase(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const {
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
    return PurchaseOrderItemMutationResult::success(fasc::server::controllers::dto::PurchaseOrderItemMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
