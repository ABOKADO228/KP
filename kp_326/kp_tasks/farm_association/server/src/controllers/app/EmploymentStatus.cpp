#include <controllers/app/EmploymentStatus.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::EmploymentStatusEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::EmploymentStatusUpdateDto& dto) {
  if (dto.name.has_value()) {
    entity.name = *dto.name;
  }
}

} // namespace

EmploymentStatusController::EmploymentStatusController(fasc::server::database::Database& db) : db_(db) {}

EmploymentStatusRowsResult EmploymentStatusController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::EmploymentStatusRowsDto dto;
    dto.rows = std::move(rows);
    return EmploymentStatusRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return EmploymentStatusRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusRowResult EmploymentStatusController::load(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const {
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
      return EmploymentStatusRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return EmploymentStatusRowResult::success(fasc::server::controllers::dto::EmploymentStatusRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return EmploymentStatusRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusMutationResult EmploymentStatusController::create(const fasc::server::controllers::dto::EmploymentStatusCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.id.has_value()) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    entity.id = *dto.id;
    hasWritableValues = true;
  }
  if (dto.name.has_value()) {
    entity.name = *dto.name;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return EmploymentStatusMutationResult::success(fasc::server::controllers::dto::EmploymentStatusMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusMutationResult EmploymentStatusController::update(
    const fasc::server::controllers::dto::EmploymentStatusKeyDto& key,
    const fasc::server::controllers::dto::EmploymentStatusUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.name.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return EmploymentStatusMutationResult::failure(
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
    return EmploymentStatusMutationResult::success(fasc::server::controllers::dto::EmploymentStatusMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusMutationResult EmploymentStatusController::erase(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const {
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
    return EmploymentStatusMutationResult::success(fasc::server::controllers::dto::EmploymentStatusMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
