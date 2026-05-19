#include <controllers/app/AssociationMember.hpp>

#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

using Entity = fasc::server::persistence::AssociationMemberEntity;

bool matchesKey(const Entity& entity, const fasc::server::controllers::dto::AssociationMemberKeyDto& key) {
  return entity.id == key.id;
}

void applyUpdateDto(Entity& entity, const fasc::server::controllers::dto::AssociationMemberUpdateDto& dto) {
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
  }
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
  }
  if (dto.membershipNumber.has_value()) {
    entity.membershipNumber = *dto.membershipNumber;
  }
  if (dto.joinedDate.has_value()) {
    entity.joinedDate = *dto.joinedDate;
  }
  if (dto.status.has_value()) {
    entity.status = *dto.status;
  }
  if (dto.notes.has_value()) {
    entity.notes = *dto.notes;
  }
}

} // namespace

AssociationMemberController::AssociationMemberController(fasc::server::database::Database& db) : db_(db) {}

AssociationMemberRowsResult AssociationMemberController::list() const {
  try {
    auto rows = db_.invokeTransactionally([&] {
      return db_.selectEntities<Entity>();
    });

    fasc::server::controllers::dto::AssociationMemberRowsDto dto;
    dto.rows = std::move(rows);
    return AssociationMemberRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationMemberRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberRowResult AssociationMemberController::load(const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const {
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
      return AssociationMemberRowResult::failure(
          FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationMemberRowResult::success(fasc::server::controllers::dto::AssociationMemberRowDto{std::move(*row)});
  } catch (const std::exception& exception) {
    return AssociationMemberRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberMutationResult AssociationMemberController::create(const fasc::server::controllers::dto::AssociationMemberCreateDto& dto) const {
  Entity entity{};
  bool hasWritableValues = false;
  if (!dto.associationId.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    entity.associationId = *dto.associationId;
    hasWritableValues = true;
  }
  if (!dto.personId.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    entity.personId = *dto.personId;
    hasWritableValues = true;
  }
  if (dto.membershipNumber.has_value()) {
    entity.membershipNumber = *dto.membershipNumber;
    hasWritableValues = true;
  }
  if (!dto.joinedDate.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: joined_date"});
  }
  if (dto.joinedDate.has_value()) {
    entity.joinedDate = *dto.joinedDate;
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
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      db_.persistEntity(entity);
      return 1ULL;
    });
    return AssociationMemberMutationResult::success(fasc::server::controllers::dto::AssociationMemberMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberMutationResult AssociationMemberController::update(
    const fasc::server::controllers::dto::AssociationMemberKeyDto& key,
    const fasc::server::controllers::dto::AssociationMemberUpdateDto& dto) const {
  bool hasWritableValues = false;
  if (dto.associationId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.personId.has_value()) {
    hasWritableValues = true;
  }
  if (dto.membershipNumber.has_value()) {
    hasWritableValues = true;
  }
  if (dto.joinedDate.has_value()) {
    hasWritableValues = true;
  }
  if (dto.status.has_value()) {
    hasWritableValues = true;
  }
  if (dto.notes.has_value()) {
    hasWritableValues = true;
  }
  if (!hasWritableValues) {
    return AssociationMemberMutationResult::failure(
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
    return AssociationMemberMutationResult::success(fasc::server::controllers::dto::AssociationMemberMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberMutationResult AssociationMemberController::erase(const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const {
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
    return AssociationMemberMutationResult::success(fasc::server::controllers::dto::AssociationMemberMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
