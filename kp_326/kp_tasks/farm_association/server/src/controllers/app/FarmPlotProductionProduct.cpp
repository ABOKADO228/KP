#include <controllers/app/FarmPlotProductionProduct.hpp>

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

fasc::server::persistence::FarmPlotProductionProductEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmPlotProductionProductEntity entity;
  entity.productId = std::stoi(requireColumn(row, "product_id"));
  entity.farmPlotId = std::stoi(requireColumn(row, "farm_plot_id"));
  entity.quantity = std::stoi(requireColumn(row, "quantity"));
  entity.productionNow = std::stoi(requireColumn(row, "production_now"));
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.productId), false});
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.farmPlotId), false});
  return values;
}

} // namespace

FarmPlotProductionProductController::FarmPlotProductionProductController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotProductionProductRowsResult FarmPlotProductionProductController::list() const {
  static const std::vector<std::string> columns{"product_id", "farm_plot_id", "quantity", "production_now"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_plot_production_product";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::FarmPlotProductionProductRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmPlotProductionProductRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductRowResult FarmPlotProductionProductController::load(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const {
  static const std::vector<std::string> columns{"product_id", "farm_plot_id", "quantity", "production_now"};
  static const std::vector<std::string> keys{"product_id", "farm_plot_id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.farm_plot_production_product WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return FarmPlotProductionProductRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotProductionProductRowResult::success(
        fasc::server::controllers::dto::FarmPlotProductionProductRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductMutationResult FarmPlotProductionProductController::create(
    const fasc::server::controllers::dto::FarmPlotProductionProductCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.productId.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productId), false});
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotId), false});
  }
  if (!dto.quantity.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (!dto.productionNow.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: production_now"});
  }
  if (dto.productionNow.has_value()) {
    columns.push_back("production_now");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productionNow), false});
  }
  if (columns.empty()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.farm_plot_production_product (" + columnsSql(columns) + ") VALUES (";
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
    return FarmPlotProductionProductMutationResult::success(
        fasc::server::controllers::dto::FarmPlotProductionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductMutationResult FarmPlotProductionProductController::update(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotProductionProductUpdateDto& dto) const {
  static const std::vector<std::string> keys{"product_id", "farm_plot_id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (dto.productionNow.has_value()) {
    columns.push_back("production_now");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productionNow), false});
  }
  if (columns.empty()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.farm_plot_production_product SET ";
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
    return FarmPlotProductionProductMutationResult::success(
        fasc::server::controllers::dto::FarmPlotProductionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotProductionProductMutationResult FarmPlotProductionProductController::erase(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const {
  static const std::vector<std::string> keys{"product_id", "farm_plot_id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.farm_plot_production_product WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return FarmPlotProductionProductMutationResult::success(
        fasc::server::controllers::dto::FarmPlotProductionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
