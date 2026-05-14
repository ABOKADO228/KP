#include <controllers/app/FarmPlotProductionProduct.hpp>

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


fasc::server::persistence::FarmPlotProductionProductEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmPlotProductionProductEntity entity;
  entity.productId = fasc::server::database::requireColumn<std::uint64_t>(row, "product_id");
  entity.farmPlotId = fasc::server::database::requireColumn<std::uint64_t>(row, "farm_plot_id");
  entity.quantity = fasc::server::database::requireColumn<int>(row, "quantity");
  entity.productionNow = fasc::server::database::requireColumn<int>(row, "production_now");
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.productId));
  values.push_back(fasc::server::database::makeSqlParameter(key.farmPlotId));
  return values;
}

} // namespace

FarmPlotProductionProductController::FarmPlotProductionProductController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotProductionProductRowsResult FarmPlotProductionProductController::list() const {
  static const std::vector<std::string> columns{"product_id", "farm_plot_id", "quantity", "production_now"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_plot_production_product", columns);
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
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_plot_production_product", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmPlotProductionProductRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotProductionProductRowResult::success(
        fasc::server::controllers::dto::FarmPlotProductionProductRowDto{rowToEntity(*row)});
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productId));
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmPlotId));
  }
  if (!dto.quantity.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.quantity));
  }
  if (!dto.productionNow.has_value()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: production_now"});
  }
  if (dto.productionNow.has_value()) {
    columns.push_back("production_now");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productionNow));
  }
  if (columns.empty()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_plot_production_product", columns, values);
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.quantity));
  }
  if (dto.productionNow.has_value()) {
    columns.push_back("production_now");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productionNow));
  }
  if (columns.empty()) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_plot_production_product", columns, values, keys, keyValues(key));
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
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_plot_production_product", keys, keyValues(key));
    });
    return FarmPlotProductionProductMutationResult::success(
        fasc::server::controllers::dto::FarmPlotProductionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotProductionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
