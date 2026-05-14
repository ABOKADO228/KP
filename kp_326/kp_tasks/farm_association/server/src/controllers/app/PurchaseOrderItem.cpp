#include <controllers/app/PurchaseOrderItem.hpp>

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

fasc::server::persistence::PurchaseOrderItemEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::PurchaseOrderItemEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.purchaseOrderId = std::stoi(requireColumn(row, "purchase_order_id"));
  entity.productId = std::stoi(requireColumn(row, "product_id"));
  if (const auto value = optionalColumn(row, "quantity")) {
    entity.quantity = std::stod(*value);
  } else {
    entity.quantity.reset();
  }
  if (const auto value = optionalColumn(row, "unit_price")) {
    entity.unitPrice = std::stod(*value);
  } else {
    entity.unitPrice.reset();
  }
  if (const auto value = optionalColumn(row, "vat_rate")) {
    entity.vatRate = std::stod(*value);
  } else {
    entity.vatRate.reset();
  }
  if (const auto value = optionalColumn(row, "currency")) {
    entity.currency = *value;
  } else {
    entity.currency.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

PurchaseOrderItemController::PurchaseOrderItemController(fasc::server::database::Database& db) : db_(db) {}

PurchaseOrderItemRowsResult PurchaseOrderItemController::list() const {
  static const std::vector<std::string> columns{"id", "purchase_order_id", "product_id", "quantity", "unit_price", "vat_rate", "currency"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.purchase_order_item";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::PurchaseOrderItemRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return PurchaseOrderItemRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PurchaseOrderItemRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemRowResult PurchaseOrderItemController::load(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "purchase_order_id", "product_id", "quantity", "unit_price", "vat_rate", "currency"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.purchase_order_item WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return PurchaseOrderItemRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseOrderItemRowResult::success(
        fasc::server::controllers::dto::PurchaseOrderItemRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemMutationResult PurchaseOrderItemController::create(
    const fasc::server::controllers::dto::PurchaseOrderItemCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.purchaseOrderId.has_value()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: purchase_order_id"});
  }
  if (dto.purchaseOrderId.has_value()) {
    columns.push_back("purchase_order_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.purchaseOrderId), false});
  }
  if (!dto.productId.has_value()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productId), false});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (dto.unitPrice.has_value()) {
    columns.push_back("unit_price");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.unitPrice), false});
  }
  if (dto.vatRate.has_value()) {
    columns.push_back("vat_rate");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.vatRate), false});
  }
  if (dto.currency.has_value()) {
    columns.push_back("currency");
    values.push_back(fasc::server::database::SqlParameter{*dto.currency, false});
  }
  if (columns.empty()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.purchase_order_item (" + columnsSql(columns) + ") VALUES (";
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
    return PurchaseOrderItemMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderItemMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemMutationResult PurchaseOrderItemController::update(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key,
    const fasc::server::controllers::dto::PurchaseOrderItemUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.purchaseOrderId.has_value()) {
    columns.push_back("purchase_order_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.purchaseOrderId), false});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productId), false});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (dto.unitPrice.has_value()) {
    columns.push_back("unit_price");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.unitPrice), false});
  }
  if (dto.vatRate.has_value()) {
    columns.push_back("vat_rate");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.vatRate), false});
  }
  if (dto.currency.has_value()) {
    columns.push_back("currency");
    values.push_back(fasc::server::database::SqlParameter{*dto.currency, false});
  }
  if (columns.empty()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.purchase_order_item SET ";
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
    return PurchaseOrderItemMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderItemMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderItemMutationResult PurchaseOrderItemController::erase(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.purchase_order_item WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return PurchaseOrderItemMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderItemMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
