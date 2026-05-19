#include <controllers/app/Person.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::PersonEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::PersonKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::PersonUpdateDto& dto) {
  if (dto.firstName.has_value()) {
    entity.firstName = *dto.firstName;
  }
  if (dto.lastName.has_value()) {
    entity.lastName = *dto.lastName;
  }
  if (dto.middleName.has_value()) {
    entity.middleName = *dto.middleName;
  }
  if (dto.birthDate.has_value()) {
    entity.birthDate = *dto.birthDate;
  }
  if (dto.phone.has_value()) {
    entity.phone = *dto.phone;
  }
  if (dto.email.has_value()) {
    entity.email = *dto.email;
  }
  if (dto.address.has_value()) {
    entity.address = *dto.address;
  }
}

} // namespace

PersonController::PersonController(fasc::server::database::Database& db) : db_(db) {}

PersonRowsResult PersonController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::PersonRowsDto dto;
    dto.rows = std::move(rows);
    return PersonRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PersonRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonRowResult PersonController::load(const fasc::server::controllers::dto::PersonKeyDto& key) const {
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
      return PersonRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PersonRowResult::success(fasc::server::controllers::dto::PersonRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return PersonRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonMutationResult PersonController::create(const fasc::server::controllers::dto::PersonCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (dto.firstName.has_value()) {
    entity.firstName = *dto.firstName;
    hasWritableValues = true;
  }
  if (dto.lastName.has_value()) {
    entity.lastName = *dto.lastName;
    hasWritableValues = true;
  }
  if (dto.middleName.has_value()) {
    entity.middleName = *dto.middleName;
    hasWritableValues = true;
  }
  if (dto.birthDate.has_value()) {
    entity.birthDate = *dto.birthDate;
    hasWritableValues = true;
  }
  if (dto.phone.has_value()) {
    entity.phone = *dto.phone;
    hasWritableValues = true;
  }
  if (dto.email.has_value()) {
    entity.email = *dto.email;
    hasWritableValues = true;
  }
  if (dto.address.has_value()) {
    entity.address = *dto.address;
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return PersonMutationResult::success(fasc::server::controllers::dto::PersonMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonMutationResult PersonController::update(
    const fasc::server::controllers::dto::PersonKeyDto& key,
    const fasc::server::controllers::dto::PersonUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.firstName.has_value()) {
    hasWritableValues = true;
  }
  if (dto.lastName.has_value()) {
    hasWritableValues = true;
  }
  if (dto.middleName.has_value()) {
    hasWritableValues = true;
  }
  if (dto.birthDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.phone.has_value()) {
    hasWritableValues = true;
  }
  if (dto.email.has_value()) {
    hasWritableValues = true;
  }
  if (dto.address.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return PersonMutationResult::failure(
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
    return PersonMutationResult::success(fasc::server::controllers::dto::PersonMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonMutationResult PersonController::erase(const fasc::server::controllers::dto::PersonKeyDto& key) const {
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
    return PersonMutationResult::success(fasc::server::controllers::dto::PersonMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
