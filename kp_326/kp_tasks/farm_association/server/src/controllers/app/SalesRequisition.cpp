#include <controllers/app/SalesRequisition.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::SalesRequisitionEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::SalesRequisitionUpdateDto& dto) {
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
  }
  if (dto.pricePerUnit.has_value()) {
    entity.pricePerUnit = *dto.pricePerUnit;
  }
  if (dto.offerDate.has_value()) {
    entity.offerDate = *dto.offerDate;
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

SalesRequisitionController::SalesRequisitionController(fasc::server::database::Database& db) : db_(db) {}

SalesRequisitionRowsResult SalesRequisitionController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::SalesRequisitionRowsDto dto;
    dto.rows = std::move(rows);
    return SalesRequisitionRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return SalesRequisitionRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionRowResult SalesRequisitionController::load(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const {
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
      return SalesRequisitionRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return SalesRequisitionRowResult::success(fasc::server::controllers::dto::SalesRequisitionRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return SalesRequisitionRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionMutationResult SalesRequisitionController::create(const fasc::server::controllers::dto::SalesRequisitionCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
    hasWritableValues = true;
  }
  if (!dto.productId.has_value()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    entity.productId = *dto.productId;
    hasWritableValues = true;
  }
  if (!dto.quantity.has_value()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    entity.quantity = *dto.quantity;
    hasWritableValues = true;
  }
  if (dto.pricePerUnit.has_value()) {
    entity.pricePerUnit = *dto.pricePerUnit;
    hasWritableValues = true;
  }
  if (!dto.offerDate.has_value()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: offer_date"});
  }
  if (dto.offerDate.has_value()) {
    entity.offerDate = *dto.offerDate;
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
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return SalesRequisitionMutationResult::success(fasc::server::controllers::dto::SalesRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionMutationResult SalesRequisitionController::update(
    const fasc::server::controllers::dto::SalesRequisitionKeyDto& key,
    const fasc::server::controllers::dto::SalesRequisitionUpdateDto& dto) const {
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
  if (dto.pricePerUnit.has_value()) {
    hasWritableValues = true;
  }
  if (dto.offerDate.has_value()) {
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
    return SalesRequisitionMutationResult::failure(
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
    return SalesRequisitionMutationResult::success(fasc::server::controllers::dto::SalesRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionMutationResult SalesRequisitionController::erase(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const {
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
    return SalesRequisitionMutationResult::success(fasc::server::controllers::dto::SalesRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
