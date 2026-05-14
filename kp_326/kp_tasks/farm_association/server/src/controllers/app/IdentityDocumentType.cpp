#include <controllers/app/IdentityDocumentType.hpp>

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

fasc::server::persistence::IdentityDocumentTypeEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::IdentityDocumentTypeEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "code")) {
    entity.code = *value;
  } else {
    entity.code.reset();
  }
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "description")) {
    entity.description = *value;
  } else {
    entity.description.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

IdentityDocumentTypeController::IdentityDocumentTypeController(fasc::server::database::Database& db) : db_(db) {}

IdentityDocumentTypeRowsResult IdentityDocumentTypeController::list() const {
  static const std::vector<std::string> columns{"id", "code", "name", "description"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.identity_document_type";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::IdentityDocumentTypeRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return IdentityDocumentTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeRowResult IdentityDocumentTypeController::load(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "code", "name", "description"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.identity_document_type WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return IdentityDocumentTypeRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return IdentityDocumentTypeRowResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::create(
    const fasc::server::controllers::dto::IdentityDocumentTypeCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    columns.push_back("id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.id), false});
  }
  if (dto.code.has_value()) {
    columns.push_back("code");
    values.push_back(fasc::server::database::SqlParameter{*dto.code, false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::SqlParameter{*dto.description, false});
  }
  if (columns.empty()) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.identity_document_type (" + columnsSql(columns) + ") VALUES (";
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
    return IdentityDocumentTypeMutationResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::update(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key,
    const fasc::server::controllers::dto::IdentityDocumentTypeUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.code.has_value()) {
    columns.push_back("code");
    values.push_back(fasc::server::database::SqlParameter{*dto.code, false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::SqlParameter{*dto.description, false});
  }
  if (columns.empty()) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.identity_document_type SET ";
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
    return IdentityDocumentTypeMutationResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::erase(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.identity_document_type WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return IdentityDocumentTypeMutationResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
