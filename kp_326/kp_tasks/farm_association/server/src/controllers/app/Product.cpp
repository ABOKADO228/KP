#include <controllers/app/Product.hpp>

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

fasc::server::persistence::ProductEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::ProductEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.typeId = std::stoi(requireColumn(row, "type_id"));
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "unit_id")) {
    entity.unitId = std::stoi(*value);
  } else {
    entity.unitId.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::ProductKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

ProductController::ProductController(fasc::server::database::Database& db) : db_(db) {}

ProductRowsResult ProductController::list() const {
  static const std::vector<std::string> columns{"id", "type_id", "name", "unit_id"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.product";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::ProductRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return ProductRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ProductRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductRowResult ProductController::load(const fasc::server::controllers::dto::ProductKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "type_id", "name", "unit_id"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.product WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return ProductRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ProductRowResult::success(
        fasc::server::controllers::dto::ProductRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return ProductRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::create(
    const fasc::server::controllers::dto::ProductCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.typeId.has_value()) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: type_id"});
  }
  if (dto.typeId.has_value()) {
    columns.push_back("type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.typeId), false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.unitId.has_value()) {
    columns.push_back("unit_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.unitId), false});
  }
  if (columns.empty()) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.product (" + columnsSql(columns) + ") VALUES (";
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
    return ProductMutationResult::success(
        fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::update(
    const fasc::server::controllers::dto::ProductKeyDto& key,
    const fasc::server::controllers::dto::ProductUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.typeId.has_value()) {
    columns.push_back("type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.typeId), false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.unitId.has_value()) {
    columns.push_back("unit_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.unitId), false});
  }
  if (columns.empty()) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.product SET ";
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
    return ProductMutationResult::success(
        fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::erase(
    const fasc::server::controllers::dto::ProductKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.product WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return ProductMutationResult::success(
        fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
