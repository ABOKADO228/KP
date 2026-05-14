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
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.purchase_order", columns);
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
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.purchase_order", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return PurchaseOrderRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseOrderRowResult::success(
        fasc::server::controllers::dto::PurchaseOrderRowDto{rowToEntity(*row)});
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
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.purchase_order", columns, values);
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
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.purchase_order", columns, values, keys, keyValues(key));
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
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.purchase_order", keys, keyValues(key));
    });
    return PurchaseOrderMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
