#include <controllers/app/AssociationEmployee.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::AssociationEmployeeEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::AssociationEmployeeUpdateDto& dto) {
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
  }
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
  }
  if (dto.roleId.has_value()) {
    entity.roleId = *dto.roleId;
  }
  if (dto.employmentStatusId.has_value()) {
    entity.employmentStatusId = *dto.employmentStatusId;
  }
  if (dto.hireDate.has_value()) {
    entity.hireDate = *dto.hireDate;
  }
  if (dto.dismissalDate.has_value()) {
    entity.dismissalDate = *dto.dismissalDate;
  }
  if (dto.salary.has_value()) {
    entity.salary = *dto.salary;
  }
  if (dto.contractNumber.has_value()) {
    entity.contractNumber = *dto.contractNumber;
  }
}

} // namespace

AssociationEmployeeController::AssociationEmployeeController(fasc::server::database::Database& db) : db_(db) {}

AssociationEmployeeRowsResult AssociationEmployeeController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::AssociationEmployeeRowsDto dto;
    dto.rows = std::move(rows);
    return AssociationEmployeeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationEmployeeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeRowResult AssociationEmployeeController::load(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const {
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
      return AssociationEmployeeRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationEmployeeRowResult::success(fasc::server::controllers::dto::AssociationEmployeeRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return AssociationEmployeeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeMutationResult AssociationEmployeeController::create(const fasc::server::controllers::dto::AssociationEmployeeCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.personId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
    hasWritableValues = true;
  }
  if (!dto.associationId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
    hasWritableValues = true;
  }
  if (!dto.roleId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: role_id"});
  }
  if (dto.roleId.has_value()) {
    entity.roleId = *dto.roleId;
    hasWritableValues = true;
  }
  if (!dto.employmentStatusId.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: employment_status_id"});
  }
  if (dto.employmentStatusId.has_value()) {
    entity.employmentStatusId = *dto.employmentStatusId;
    hasWritableValues = true;
  }
  if (!dto.hireDate.has_value()) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: hire_date"});
  }
  if (dto.hireDate.has_value()) {
    entity.hireDate = *dto.hireDate;
    hasWritableValues = true;
  }
  if (dto.dismissalDate.has_value()) {
    entity.dismissalDate = *dto.dismissalDate;
    hasWritableValues = true;
  }
  if (dto.salary.has_value()) {
    entity.salary = *dto.salary;
    hasWritableValues = true;
  }
  if (dto.contractNumber.has_value()) {
    entity.contractNumber = *dto.contractNumber;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return AssociationEmployeeMutationResult::success(fasc::server::controllers::dto::AssociationEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeMutationResult AssociationEmployeeController::update(
    const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key,
    const fasc::server::controllers::dto::AssociationEmployeeUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.personId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.associationId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.roleId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.employmentStatusId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.hireDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.dismissalDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.salary.has_value()) {
    hasWritableValues = true;
  }
  if (dto.contractNumber.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return AssociationEmployeeMutationResult::failure(
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
    return AssociationEmployeeMutationResult::success(fasc::server::controllers::dto::AssociationEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationEmployeeMutationResult AssociationEmployeeController::erase(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const {
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
    return AssociationEmployeeMutationResult::success(fasc::server::controllers::dto::AssociationEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
