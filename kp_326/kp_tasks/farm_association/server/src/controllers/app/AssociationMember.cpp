#include <controllers/app/AssociationMember.hpp>

#include <database/SqlValue.hpp>

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace fasc::server::controllers::app {

namespace {

std::string requireColumn(const fasc::server::database::SqlRow& row, const std::string& column) {
  const auto it = row.find(column);
  if (it == row.end() || !it->second.has_value()) {
    throw std::runtime_error{"Column is null: " + column};
  }
  return *it->second;
}

std::optional<std::string> optionalColumn(const fasc::server::database::SqlRow& row,
                                         const std::string& column) {
  const auto it = row.find(column);
  if (it == row.end()) {
    throw std::runtime_error{"Column is missing: " + column};
  }
  return it->second;
}


fasc::server::persistence::AssociationMemberEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::AssociationMemberEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  entity.associationId = fasc::server::database::requireColumn<std::uint64_t>(row, "association_id");
  entity.personId = fasc::server::database::requireColumn<std::uint64_t>(row, "person_id");
  if (const auto value = optionalColumn(row, "membership_number")) {
    entity.membershipNumber = *value;
  } else {
    entity.membershipNumber.reset();
  }
  entity.joinedDate = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "joined_date");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::AssociationMemberStatus>(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "notes")) {
    entity.notes = *value;
  } else {
    entity.notes.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::AssociationMemberKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

AssociationMemberController::AssociationMemberController(fasc::server::database::Database& db) : db_(db) {}

AssociationMemberRowsResult AssociationMemberController::list() const {
  static const std::vector<std::string> columns{"id", "association_id", "person_id", "membership_number", "joined_date", "status", "notes"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.association_member", columns);
    });

    fasc::server::controllers::dto::AssociationMemberRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return AssociationMemberRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationMemberRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberRowResult AssociationMemberController::load(const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "association_id", "person_id", "membership_number", "joined_date", "status", "notes"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.association_member", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return AssociationMemberRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationMemberRowResult::success(
        fasc::server::controllers::dto::AssociationMemberRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return AssociationMemberRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberMutationResult AssociationMemberController::create(
    const fasc::server::controllers::dto::AssociationMemberCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.associationId.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.associationId));
  }
  if (!dto.personId.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.personId));
  }
  if (dto.membershipNumber.has_value()) {
    columns.push_back("membership_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.membershipNumber));
  }
  if (!dto.joinedDate.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: joined_date"});
  }
  if (dto.joinedDate.has_value()) {
    columns.push_back("joined_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.joinedDate));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.association_member", columns, values);
    });
    return AssociationMemberMutationResult::success(
        fasc::server::controllers::dto::AssociationMemberMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberMutationResult AssociationMemberController::update(
    const fasc::server::controllers::dto::AssociationMemberKeyDto& key,
    const fasc::server::controllers::dto::AssociationMemberUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.associationId));
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.personId));
  }
  if (dto.membershipNumber.has_value()) {
    columns.push_back("membership_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.membershipNumber));
  }
  if (dto.joinedDate.has_value()) {
    columns.push_back("joined_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.joinedDate));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.association_member", columns, values, keys, keyValues(key));
    });
    return AssociationMemberMutationResult::success(
        fasc::server::controllers::dto::AssociationMemberMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationMemberMutationResult AssociationMemberController::erase(
    const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.association_member", keys, keyValues(key));
    });
    return AssociationMemberMutationResult::success(
        fasc::server::controllers::dto::AssociationMemberMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
