#include <controllers/app/FarmPlotConsumptionProduct.hpp>

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


fasc::server::persistence::FarmPlotConsumptionProductEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmPlotConsumptionProductEntity entity;
  entity.productId = std::stoi(requireColumn(row, "product_id"));
  entity.farmPlotId = std::stoi(requireColumn(row, "farm_plot_id"));
  entity.quantity = std::stoi(requireColumn(row, "quantity"));
  entity.consumptionNow = std::stoi(requireColumn(row, "consumption_now"));
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.productId), false});
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.farmPlotId), false});
  return values;
}

} // namespace

FarmPlotConsumptionProductController::FarmPlotConsumptionProductController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotConsumptionProductRowsResult FarmPlotConsumptionProductController::list() const {
  static const std::vector<std::string> columns{"product_id", "farm_plot_id", "quantity", "consumption_now"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_plot_consumption_product", columns);
    });

    fasc::server::controllers::dto::FarmPlotConsumptionProductRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmPlotConsumptionProductRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductRowResult FarmPlotConsumptionProductController::load(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const {
  static const std::vector<std::string> columns{"product_id", "farm_plot_id", "quantity", "consumption_now"};
  static const std::vector<std::string> keys{"product_id", "farm_plot_id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_plot_consumption_product", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmPlotConsumptionProductRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotConsumptionProductRowResult::success(
        fasc::server::controllers::dto::FarmPlotConsumptionProductRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductMutationResult FarmPlotConsumptionProductController::create(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.productId.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productId), false});
  }
  if (!dto.farmPlotId.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_id"});
  }
  if (dto.farmPlotId.has_value()) {
    columns.push_back("farm_plot_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotId), false});
  }
  if (!dto.quantity.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (!dto.consumptionNow.has_value()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: consumption_now"});
  }
  if (dto.consumptionNow.has_value()) {
    columns.push_back("consumption_now");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.consumptionNow), false});
  }
  if (columns.empty()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_plot_consumption_product", columns, values);
    });
    return FarmPlotConsumptionProductMutationResult::success(
        fasc::server::controllers::dto::FarmPlotConsumptionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductMutationResult FarmPlotConsumptionProductController::update(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotConsumptionProductUpdateDto& dto) const {
  static const std::vector<std::string> keys{"product_id", "farm_plot_id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (dto.consumptionNow.has_value()) {
    columns.push_back("consumption_now");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.consumptionNow), false});
  }
  if (columns.empty()) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_plot_consumption_product", columns, values, keys, keyValues(key));
    });
    return FarmPlotConsumptionProductMutationResult::success(
        fasc::server::controllers::dto::FarmPlotConsumptionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotConsumptionProductMutationResult FarmPlotConsumptionProductController::erase(
    const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const {
  static const std::vector<std::string> keys{"product_id", "farm_plot_id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_plot_consumption_product", keys, keyValues(key));
    });
    return FarmPlotConsumptionProductMutationResult::success(
        fasc::server::controllers::dto::FarmPlotConsumptionProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotConsumptionProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
