#include <controllers/app/AssociationMember.hpp>

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

std::string columnsSql(const std::vector<std::string>& columns) {
  std::string sql;
  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i != 0) {
      sql += ", ";
    }
    sql += columns[i];
  }
  return sql;
}

std::string whereSql(const std::vector<std::string>& keys, std::size_t offset = 0) {
  std::string sql;
  for (std::size_t i = 0; i < keys.size(); ++i) {
    if (i != 0) {
      sql += " AND ";
    }
    sql += keys[i] + " = $" + std::to_string(i + 1 + offset);
  }
  return sql;
}

fasc::server::persistence::AssociationMemberEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::AssociationMemberEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.associationId = std::stoi(requireColumn(row, "association_id"));
  entity.personId = std::stoi(requireColumn(row, "person_id"));
  if (const auto value = optionalColumn(row, "membership_number")) {
    entity.membershipNumber = *value;
  } else {
    entity.membershipNumber.reset();
  }
  entity.joinedDate = requireColumn(row, "joined_date");
  if (const auto value = optionalColumn(row, "status")) {
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
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

AssociationMemberController::AssociationMemberController(fasc::server::database::Database& db) : db_(db) {}

AssociationMemberRowsResult AssociationMemberController::list() const {
  static const std::vector<std::string> columns{"id", "association_id", "person_id", "membership_number", "joined_date", "status", "notes"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.association_member";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
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
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.association_member WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return AssociationMemberRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationMemberRowResult::success(
        fasc::server::controllers::dto::AssociationMemberRowDto{rowToEntity(rows.front())});
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
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (!dto.personId.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (dto.membershipNumber.has_value()) {
    columns.push_back("membership_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.membershipNumber, false});
  }
  if (!dto.joinedDate.has_value()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: joined_date"});
  }
  if (dto.joinedDate.has_value()) {
    columns.push_back("joined_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.joinedDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.association_member (" + columnsSql(columns) + ") VALUES (";
    for (std::size_t i = 0; i < values.size(); ++i) {
      if (i != 0) {
        sql += ", ";
      }
      sql += "$" + std::to_string(i + 1);
    }
    sql += ")";
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
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
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (dto.membershipNumber.has_value()) {
    columns.push_back("membership_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.membershipNumber, false});
  }
  if (dto.joinedDate.has_value()) {
    columns.push_back("joined_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.joinedDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.association_member SET ";
    for (std::size_t i = 0; i < columns.size(); ++i) {
      if (i != 0) {
        sql += ", ";
      }
      sql += columns[i] + " = $" + std::to_string(i + 1);
    }
    sql += " WHERE " + whereSql(keys, values.size());
    const std::vector<fasc::server::database::SqlParameter> keyParams = keyValues(key);
    values.insert(values.end(), keyParams.begin(), keyParams.end());
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
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
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.association_member WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return AssociationMemberMutationResult::success(
        fasc::server::controllers::dto::AssociationMemberMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationMemberMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
