#include <controllers/app/FarmEmployee.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::FarmEmployeeEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::FarmEmployeeUpdateDto& dto) {
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
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
  if (dto.employmentContractNumber.has_value()) {
    entity.employmentContractNumber = *dto.employmentContractNumber;
  }
  if (dto.isPrimaryWorkplace.has_value()) {
    entity.isPrimaryWorkplace = *dto.isPrimaryWorkplace;
  }
}

} // namespace

FarmEmployeeController::FarmEmployeeController(fasc::server::database::Database& db) : db_(db) {}

FarmEmployeeRowsResult FarmEmployeeController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::FarmEmployeeRowsDto dto;
    dto.rows = std::move(rows);
    return FarmEmployeeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmEmployeeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeRowResult FarmEmployeeController::load(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const {
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
      return FarmEmployeeRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmEmployeeRowResult::success(fasc::server::controllers::dto::FarmEmployeeRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return FarmEmployeeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeMutationResult FarmEmployeeController::create(const fasc::server::controllers::dto::FarmEmployeeCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.personId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
    hasWritableValues = true;
  }
  if (!dto.farmId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    entity.farmId = *dto.farmId;
    hasWritableValues = true;
  }
  if (!dto.roleId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: role_id"});
  }
  if (dto.roleId.has_value()) {
    entity.roleId = *dto.roleId;
    hasWritableValues = true;
  }
  if (!dto.employmentStatusId.has_value()) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: employment_status_id"});
  }
  if (dto.employmentStatusId.has_value()) {
    entity.employmentStatusId = *dto.employmentStatusId;
    hasWritableValues = true;
  }
  if (!dto.hireDate.has_value()) {
    return FarmEmployeeMutationResult::failure(
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
  if (dto.employmentContractNumber.has_value()) {
    entity.employmentContractNumber = *dto.employmentContractNumber;
    hasWritableValues = true;
  }
  if (dto.isPrimaryWorkplace.has_value()) {
    entity.isPrimaryWorkplace = *dto.isPrimaryWorkplace;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return FarmEmployeeMutationResult::success(fasc::server::controllers::dto::FarmEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeMutationResult FarmEmployeeController::update(
    const fasc::server::controllers::dto::FarmEmployeeKeyDto& key,
    const fasc::server::controllers::dto::FarmEmployeeUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.personId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.farmId.has_value()) {
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
  if (dto.employmentContractNumber.has_value()) {
    hasWritableValues = true;
  }
  if (dto.isPrimaryWorkplace.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return FarmEmployeeMutationResult::failure(
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
    return FarmEmployeeMutationResult::success(fasc::server::controllers::dto::FarmEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmEmployeeMutationResult FarmEmployeeController::erase(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const {
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
    return FarmEmployeeMutationResult::success(fasc::server::controllers::dto::FarmEmployeeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmEmployeeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
