#include <controllers/app/PurchaseOrder.hpp>

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

fasc::server::persistence::PurchaseOrderEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::PurchaseOrderEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.associationId = std::stoi(requireColumn(row, "association_id"));
  entity.supplierId = std::stoi(requireColumn(row, "supplier_id"));
  if (const auto value = optionalColumn(row, "delivery_address")) {
    entity.deliveryAddress = *value;
  } else {
    entity.deliveryAddress.reset();
  }
  entity.orderDate = requireColumn(row, "order_date");
  if (const auto value = optionalColumn(row, "expected_delivery_date")) {
    entity.expectedDeliveryDate = *value;
  } else {
    entity.expectedDeliveryDate.reset();
  }
  if (const auto value = optionalColumn(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "total_amount")) {
    entity.totalAmount = std::stod(*value);
  } else {
    entity.totalAmount.reset();
  }
  if (const auto value = optionalColumn(row, "received_at")) {
    entity.receivedAt = *value;
  } else {
    entity.receivedAt.reset();
  }
  if (const auto value = optionalColumn(row, "created_by")) {
    entity.createdBy = std::stoi(*value);
  } else {
    entity.createdBy.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

PurchaseOrderController::PurchaseOrderController(fasc::server::database::Database& db) : db_(db) {}

PurchaseOrderRowsResult PurchaseOrderController::list() const {
  static const std::vector<std::string> columns{"id", "association_id", "supplier_id", "delivery_address", "order_date", "expected_delivery_date", "status", "total_amount", "received_at", "created_by"};
  try {
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.purchase_order";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, {});
    });

    fasc::server::controllers::dto::PurchaseOrderRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return PurchaseOrderRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PurchaseOrderRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderRowResult PurchaseOrderController::load(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "association_id", "supplier_id", "delivery_address", "order_date", "expected_delivery_date", "status", "total_amount", "received_at", "created_by"};
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "SELECT " + columnsSql(columns) + " FROM public.purchase_order WHERE " +
                            whereSql(keys) + " LIMIT 1";
    const auto rows = db_.invokeTransactionally([&] {
      return db_.querySql(sql, values);
    });
    if (rows.empty()) {
      return PurchaseOrderRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseOrderRowResult::success(
        fasc::server::controllers::dto::PurchaseOrderRowDto{rowToEntity(rows.front())});
  } catch (const std::exception& exception) {
    return PurchaseOrderRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderMutationResult PurchaseOrderController::create(
    const fasc::server::controllers::dto::PurchaseOrderCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.associationId.has_value()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (!dto.supplierId.has_value()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: supplier_id"});
  }
  if (dto.supplierId.has_value()) {
    columns.push_back("supplier_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.supplierId), false});
  }
  if (dto.deliveryAddress.has_value()) {
    columns.push_back("delivery_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.deliveryAddress, false});
  }
  if (!dto.orderDate.has_value()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: order_date"});
  }
  if (dto.orderDate.has_value()) {
    columns.push_back("order_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.orderDate, false});
  }
  if (dto.expectedDeliveryDate.has_value()) {
    columns.push_back("expected_delivery_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.expectedDeliveryDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.totalAmount.has_value()) {
    columns.push_back("total_amount");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.totalAmount), false});
  }
  if (dto.receivedAt.has_value()) {
    columns.push_back("received_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.receivedAt, false});
  }
  if (dto.createdBy.has_value()) {
    columns.push_back("created_by");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.createdBy), false});
  }
  if (columns.empty()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "INSERT INTO public.purchase_order (" + columnsSql(columns) + ") VALUES (";
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
    return PurchaseOrderMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderMutationResult PurchaseOrderController::update(
    const fasc::server::controllers::dto::PurchaseOrderKeyDto& key,
    const fasc::server::controllers::dto::PurchaseOrderUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.associationId), false});
  }
  if (dto.supplierId.has_value()) {
    columns.push_back("supplier_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.supplierId), false});
  }
  if (dto.deliveryAddress.has_value()) {
    columns.push_back("delivery_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.deliveryAddress, false});
  }
  if (dto.orderDate.has_value()) {
    columns.push_back("order_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.orderDate, false});
  }
  if (dto.expectedDeliveryDate.has_value()) {
    columns.push_back("expected_delivery_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.expectedDeliveryDate, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.totalAmount.has_value()) {
    columns.push_back("total_amount");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.totalAmount), false});
  }
  if (dto.receivedAt.has_value()) {
    columns.push_back("received_at");
    values.push_back(fasc::server::database::SqlParameter{*dto.receivedAt, false});
  }
  if (dto.createdBy.has_value()) {
    columns.push_back("created_by");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.createdBy), false});
  }
  if (columns.empty()) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    std::string sql = "UPDATE public.purchase_order SET ";
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
    return PurchaseOrderMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseOrderMutationResult PurchaseOrderController::erase(
    const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const std::vector<fasc::server::database::SqlParameter> values = keyValues(key);
    const std::string sql = "DELETE FROM public.purchase_order WHERE " + whereSql(keys);
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.executeSql(sql, values);
    });
    return PurchaseOrderMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
