#include <controllers/app/PurchaseOrder.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::PurchaseOrderEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::PurchaseOrderUpdateDto& dto) {
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
  }
  if (dto.supplierId.has_value()) {
    entity.supplierId = *dto.supplierId;
  }
  if (dto.deliveryAddress.has_value()) {
    entity.deliveryAddress = *dto.deliveryAddress;
  }
  if (dto.orderDate.has_value()) {
    entity.orderDate = *dto.orderDate;
  }
  if (dto.expectedDeliveryDate.has_value()) {
    entity.expectedDeliveryDate = *dto.expectedDeliveryDate;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.totalAmount.has_value()) {
    entity.totalAmount = *dto.totalAmount;
  }
  if (dto.receivedAt.has_value()) {
    entity.receivedAt = *dto.receivedAt;
  }
  if (dto.createdBy.has_value()) {
    entity.createdBy = *dto.createdBy;
  }
}

} // namespace

PurchaseOrderController::PurchaseOrderController(fasc::server::database::Database& db) : db_(db) {}

PurchaseOrderRowsResult PurchaseOrderController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::PurchaseOrderRowsDto dto;
    dto.rows = std::move(rows);
    return PurchaseOrderRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PurchaseOrderRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderRowResult PurchaseOrderController::load(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const {
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
      return PurchaseOrderRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseOrderRowResult::success(fasc::server::controllers::dto::PurchaseOrderRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return PurchaseOrderRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderMutationResult PurchaseOrderController::create(const fasc::server::controllers::dto::PurchaseOrderCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.associationId.has_value()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
    hasWritableValues = true;
  }
  if (!dto.supplierId.has_value()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: supplier_id"});
  }
  if (dto.supplierId.has_value()) {
    entity.supplierId = *dto.supplierId;
    hasWritableValues = true;
  }
  if (dto.deliveryAddress.has_value()) {
    entity.deliveryAddress = *dto.deliveryAddress;
    hasWritableValues = true;
  }
  if (!dto.orderDate.has_value()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: order_date"});
  }
  if (dto.orderDate.has_value()) {
    entity.orderDate = *dto.orderDate;
    hasWritableValues = true;
  }
  if (dto.expectedDeliveryDate.has_value()) {
    entity.expectedDeliveryDate = *dto.expectedDeliveryDate;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (dto.totalAmount.has_value()) {
    entity.totalAmount = *dto.totalAmount;
    hasWritableValues = true;
  }
  if (dto.receivedAt.has_value()) {
    entity.receivedAt = *dto.receivedAt;
    hasWritableValues = true;
  }
  if (dto.createdBy.has_value()) {
    entity.createdBy = *dto.createdBy;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return PurchaseOrderMutationResult::success(fasc::server::controllers::dto::PurchaseOrderMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderMutationResult PurchaseOrderController::update(
    const fasc::server::controllers::dto::PurchaseOrderKeyDto& key,
    const fasc::server::controllers::dto::PurchaseOrderUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.associationId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.supplierId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.deliveryAddress.has_value()) {
    hasWritableValues = true;
  }
  if (dto.orderDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.expectedDeliveryDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.totalAmount.has_value()) {
    hasWritableValues = true;
  }
  if (dto.receivedAt.has_value()) {
    hasWritableValues = true;
  }
  if (dto.createdBy.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PurchaseOrderMutationResult::failure(
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
    return PurchaseOrderMutationResult::success(fasc::server::controllers::dto::PurchaseOrderMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderMutationResult PurchaseOrderController::erase(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const {
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
    return PurchaseOrderMutationResult::success(fasc::server::controllers::dto::PurchaseOrderMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
