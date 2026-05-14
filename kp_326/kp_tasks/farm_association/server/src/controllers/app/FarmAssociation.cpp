#include <controllers/app/FarmAssociation.hpp>

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

fasc::server::persistence::FarmAssociationEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmAssociationEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "inn")) {
    entity.inn = *value;
  } else {
    entity.inn.reset();
  }
  if (const auto value = optionalColumn(row, "ogrn")) {
    entity.ogrn = *value;
  } else {
    entity.ogrn.reset();
  }
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "legal_address")) {
    entity.legalAddress = *value;
  } else {
    entity.legalAddress.reset();
  }
  if (const auto value = optionalColumn(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmAssociationController::FarmAssociationController(fasc::server::database::Database& db) : db_(db) {}

FarmAssociationRowsResult FarmAssociationController::list() const {
  static const std::vector<std::string> columns{"id", "inn", "ogrn", "name", "legal_address", "status"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_association";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::FarmAssociationRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmAssociationRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmAssociationRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationRowResult FarmAssociationController::load(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "inn", "ogrn", "name", "legal_address", "status"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_association WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return FarmAssociationRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmAssociationRowResult::success(
        fasc::server::controllers::dto::FarmAssociationRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return FarmAssociationRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::create(
    const fasc::server::controllers::dto::FarmAssociationCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.inn.has_value()) {
    columns.push_back("inn");
    values.push_back(fasc::server::database::SqlParameter{*dto.inn, false});
  }
  if (dto.ogrn.has_value()) {
    columns.push_back("ogrn");
    values.push_back(fasc::server::database::SqlParameter{*dto.ogrn, false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalAddress, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (columns.empty()) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.farm_association (" + columnsSql(columns) + ") VALUES (";
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
    return FarmAssociationMutationResult::success(
        fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::update(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key,
    const fasc::server::controllers::dto::FarmAssociationUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.inn.has_value()) {
    columns.push_back("inn");
    values.push_back(fasc::server::database::SqlParameter{*dto.inn, false});
  }
  if (dto.ogrn.has_value()) {
    columns.push_back("ogrn");
    values.push_back(fasc::server::database::SqlParameter{*dto.ogrn, false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalAddress, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (columns.empty()) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.farm_association SET ";
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
    return FarmAssociationMutationResult::success(
        fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::erase(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.farm_association WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return FarmAssociationMutationResult::success(
        fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
