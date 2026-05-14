#include <controllers/app/ProductType.hpp>

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

fasc::server::persistence::ProductTypeEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::ProductTypeEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "parent_id")) {
    entity.parentId = std::stoi(*value);
  } else {
    entity.parentId.reset();
  }
  if (const auto value = optionalColumn(row, "sku")) {
    entity.sku = *value;
  } else {
    entity.sku.reset();
  }
  entity.productLevel = std::stoi(requireColumn(row, "product_level"));
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
    const fasc::server::controllers::dto::ProductTypeKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

ProductTypeController::ProductTypeController(fasc::server::database::Database& db) : db_(db) {}

ProductTypeRowsResult ProductTypeController::list() const {
  static const std::vector<std::string> columns{"id", "parent_id", "sku", "product_level", "name", "description"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.product_type";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::ProductTypeRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return ProductTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ProductTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeRowResult ProductTypeController::load(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "parent_id", "sku", "product_level", "name", "description"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.product_type WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return ProductTypeRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ProductTypeRowResult::success(
        fasc::server::controllers::dto::ProductTypeRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return ProductTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::create(
    const fasc::server::controllers::dto::ProductTypeCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    columns.push_back("id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.id), false});
  }
  if (dto.parentId.has_value()) {
    columns.push_back("parent_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.parentId), false});
  }
  if (dto.sku.has_value()) {
    columns.push_back("sku");
    values.push_back(fasc::server::database::SqlParameter{*dto.sku, false});
  }
  if (!dto.productLevel.has_value()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_level"});
  }
  if (dto.productLevel.has_value()) {
    columns.push_back("product_level");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productLevel), false});
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
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.product_type (" + columnsSql(columns) + ") VALUES (";
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
    return ProductTypeMutationResult::success(
        fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::update(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key,
    const fasc::server::controllers::dto::ProductTypeUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.parentId.has_value()) {
    columns.push_back("parent_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.parentId), false});
  }
  if (dto.sku.has_value()) {
    columns.push_back("sku");
    values.push_back(fasc::server::database::SqlParameter{*dto.sku, false});
  }
  if (dto.productLevel.has_value()) {
    columns.push_back("product_level");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productLevel), false});
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
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.product_type SET ";
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
    return ProductTypeMutationResult::success(
        fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::erase(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.product_type WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return ProductTypeMutationResult::success(
        fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
