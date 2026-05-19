#include <controllers/app/PurchaseRequisition.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::PurchaseRequisitionEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::PurchaseRequisitionUpdateDto& dto) {
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
  }
  if (dto.maxPricePerUnit.has_value()) {
    entity.maxPricePerUnit = *dto.maxPricePerUnit;
  }
  if (dto.offerDate.has_value()) {
    entity.offerDate = *dto.offerDate;
  }
  if (dto.requiredDate.has_value()) {
    entity.requiredDate = *dto.requiredDate;
  }
  if (dto.priority.has_value()) {
    entity.priority = *dto.priority;
  }
  if (dto.validUntil.has_value()) {
    entity.validUntil = *dto.validUntil;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
  }
}

} // namespace

PurchaseRequisitionController::PurchaseRequisitionController(fasc::server::database::Database& db) : db_(db) {}

PurchaseRequisitionRowsResult PurchaseRequisitionController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::PurchaseRequisitionRowsDto dto;
    dto.rows = std::move(rows);
    return PurchaseRequisitionRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PurchaseRequisitionRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionRowResult PurchaseRequisitionController::load(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const {
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
      return PurchaseRequisitionRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseRequisitionRowResult::success(fasc::server::controllers::dto::PurchaseRequisitionRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionMutationResult PurchaseRequisitionController::create(const fasc::server::controllers::dto::PurchaseRequisitionCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.farmId.has_value()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
    hasWritableValues = true;
  }
  if (!dto.productId.has_value()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
    hasWritableValues = true;
  }
  if (!dto.quantity.has_value()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
    hasWritableValues = true;
  }
  if (dto.maxPricePerUnit.has_value()) {
    entity.maxPricePerUnit = *dto.maxPricePerUnit;
    hasWritableValues = true;
  }
  if (dto.offerDate.has_value()) {
    entity.offerDate = *dto.offerDate;
    hasWritableValues = true;
  }
  if (dto.requiredDate.has_value()) {
    entity.requiredDate = *dto.requiredDate;
    hasWritableValues = true;
  }
  if (dto.priority.has_value()) {
    entity.priority = *dto.priority;
    hasWritableValues = true;
  }
  if (dto.validUntil.has_value()) {
    entity.validUntil = *dto.validUntil;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return PurchaseRequisitionMutationResult::success(fasc::server::controllers::dto::PurchaseRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionMutationResult PurchaseRequisitionController::update(
    const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key,
    const fasc::server::controllers::dto::PurchaseRequisitionUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.farmId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.productId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.quantity.has_value()) {
    hasWritableValues = true;
  }
  if (dto.maxPricePerUnit.has_value()) {
    hasWritableValues = true;
  }
  if (dto.offerDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.requiredDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.priority.has_value()) {
    hasWritableValues = true;
  }
  if (dto.validUntil.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PurchaseRequisitionMutationResult::failure(
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
    return PurchaseRequisitionMutationResult::success(fasc::server::controllers::dto::PurchaseRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionMutationResult PurchaseRequisitionController::erase(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const {
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
    return PurchaseRequisitionMutationResult::success(fasc::server::controllers::dto::PurchaseRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
