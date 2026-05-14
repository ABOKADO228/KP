#include <controllers/app/PersonDocument.hpp>

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

fasc::server::persistence::PersonDocumentEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::PersonDocumentEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.personId = std::stoi(requireColumn(row, "person_id"));
  entity.documentTypeId = std::stoi(requireColumn(row, "document_type_id"));
  if (const auto value = optionalColumn(row, "document_number")) {
    entity.documentNumber = *value;
  } else {
    entity.documentNumber.reset();
  }
  if (const auto value = optionalColumn(row, "issued_by")) {
    entity.issuedBy = *value;
  } else {
    entity.issuedBy.reset();
  }
  if (const auto value = optionalColumn(row, "issued_date")) {
    entity.issuedDate = *value;
  } else {
    entity.issuedDate.reset();
  }
  if (const auto value = optionalColumn(row, "expiration_date")) {
    entity.expirationDate = *value;
  } else {
    entity.expirationDate.reset();
  }
  if (const auto value = optionalColumn(row, "is_primary")) {
    entity.isPrimary = std::stoi(*value);
  } else {
    entity.isPrimary.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

PersonDocumentController::PersonDocumentController(fasc::server::database::Database& db) : db_(db) {}

PersonDocumentRowsResult PersonDocumentController::list() const {
  static const std::vector<std::string> columns{"id", "person_id", "document_type_id", "document_number", "issued_by", "issued_date", "expiration_date", "is_primary"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.person_document";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::PersonDocumentRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return PersonDocumentRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PersonDocumentRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentRowResult PersonDocumentController::load(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "person_id", "document_type_id", "document_number", "issued_by", "issued_date", "expiration_date", "is_primary"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.person_document WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return PersonDocumentRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PersonDocumentRowResult::success(
        fasc::server::controllers::dto::PersonDocumentRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return PersonDocumentRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::create(
    const fasc::server::controllers::dto::PersonDocumentCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.personId.has_value()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (!dto.documentTypeId.has_value()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: document_type_id"});
  }
  if (dto.documentTypeId.has_value()) {
    columns.push_back("document_type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.documentTypeId), false});
  }
  if (dto.documentNumber.has_value()) {
    columns.push_back("document_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.documentNumber, false});
  }
  if (dto.issuedBy.has_value()) {
    columns.push_back("issued_by");
    values.push_back(fasc::server::database::SqlParameter{*dto.issuedBy, false});
  }
  if (dto.issuedDate.has_value()) {
    columns.push_back("issued_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.issuedDate, false});
  }
  if (dto.expirationDate.has_value()) {
    columns.push_back("expiration_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.expirationDate, false});
  }
  if (dto.isPrimary.has_value()) {
    columns.push_back("is_primary");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.isPrimary), false});
  }
  if (columns.empty()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.person_document (" + columnsSql(columns) + ") VALUES (";
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
    return PersonDocumentMutationResult::success(
        fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::update(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key,
    const fasc::server::controllers::dto::PersonDocumentUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (dto.documentTypeId.has_value()) {
    columns.push_back("document_type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.documentTypeId), false});
  }
  if (dto.documentNumber.has_value()) {
    columns.push_back("document_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.documentNumber, false});
  }
  if (dto.issuedBy.has_value()) {
    columns.push_back("issued_by");
    values.push_back(fasc::server::database::SqlParameter{*dto.issuedBy, false});
  }
  if (dto.issuedDate.has_value()) {
    columns.push_back("issued_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.issuedDate, false});
  }
  if (dto.expirationDate.has_value()) {
    columns.push_back("expiration_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.expirationDate, false});
  }
  if (dto.isPrimary.has_value()) {
    columns.push_back("is_primary");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.isPrimary), false});
  }
  if (columns.empty()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.person_document SET ";
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
    return PersonDocumentMutationResult::success(
        fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::erase(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.person_document WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return PersonDocumentMutationResult::success(
        fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
