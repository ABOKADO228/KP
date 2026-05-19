#include <controllers/app/Contract.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::ContractEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::ContractKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::ContractUpdateDto& dto) {
  if (dto.supplierId.has_value()) {
    entity.supplierId = *dto.supplierId;
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
  }
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
  }
  if (dto.contractNumber.has_value()) {
    entity.contractNumber = *dto.contractNumber;
  }
  if (dto.signDate.has_value()) {
    entity.signDate = *dto.signDate;
  }
  if (dto.startDate.has_value()) {
    entity.startDate = *dto.startDate;
  }
  if (dto.endDate.has_value()) {
    entity.endDate = *dto.endDate;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.description.has_value()) {
    entity.description = *dto.description;
  }
}

} // namespace

ContractController::ContractController(fasc::server::database::Database& db) : db_(db) {}

ContractRowsResult ContractController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::ContractRowsDto dto;
    dto.rows = std::move(rows);
    return ContractRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ContractRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractRowResult ContractController::load(const fasc::server::controllers::dto::ContractKeyDto& key) const {
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
      return ContractRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ContractRowResult::success(fasc::server::controllers::dto::ContractRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return ContractRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractMutationResult ContractController::create(const fasc::server::controllers::dto::ContractCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (dto.supplierId.has_value()) {
    entity.supplierId = *dto.supplierId;
    hasWritableValues = true;
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
    hasWritableValues = true;
  }
  if (!dto.associationId.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
    hasWritableValues = true;
  }
  if (dto.contractNumber.has_value()) {
    entity.contractNumber = *dto.contractNumber;
    hasWritableValues = true;
  }
  if (!dto.signDate.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: sign_date"});
  }
  if (dto.signDate.has_value()) {
    entity.signDate = *dto.signDate;
    hasWritableValues = true;
  }
  if (!dto.startDate.has_value()) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: start_date"});
  }
  if (dto.startDate.has_value()) {
    entity.startDate = *dto.startDate;
    hasWritableValues = true;
  }
  if (dto.endDate.has_value()) {
    entity.endDate = *dto.endDate;
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
    hasWritableValues = true;
  }
  if (dto.description.has_value()) {
    entity.description = *dto.description;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return ContractMutationResult::success(fasc::server::controllers::dto::ContractMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractMutationResult ContractController::update(
    const fasc::server::controllers::dto::ContractKeyDto& key,
    const fasc::server::controllers::dto::ContractUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.supplierId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.farmId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.associationId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.contractNumber.has_value()) {
    hasWritableValues = true;
  }
  if (dto.signDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.startDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.endDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.description.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return ContractMutationResult::failure(
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
    return ContractMutationResult::success(fasc::server::controllers::dto::ContractMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ContractMutationResult ContractController::erase(const fasc::server::controllers::dto::ContractKeyDto& key) const {
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
    return ContractMutationResult::success(fasc::server::controllers::dto::ContractMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ContractMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
