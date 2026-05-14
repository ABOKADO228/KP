#include <controllers/app/PurchaseOrderItem.hpp>

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


fasc::server::persistence::PurchaseOrderItemEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::PurchaseOrderItemEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  entity.purchaseOrderId = fasc::server::database::requireColumn<std::uint64_t>(row, "purchase_order_id");
  entity.productId = fasc::server::database::requireColumn<std::uint64_t>(row, "product_id");
  if (const auto value = fasc::server::database::optionalColumn<double>(row, "quantity")) {
    entity.quantity = *value;
  } else {
    entity.quantity.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<double>(row, "unit_price")) {
    entity.unitPrice = *value;
  } else {
    entity.unitPrice.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<double>(row, "vat_rate")) {
    entity.vatRate = *value;
  } else {
    entity.vatRate.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::CurrencyCode>(row, "currency")) {
    entity.currency = *value;
  } else {
    entity.currency.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

PurchaseOrderItemController::PurchaseOrderItemController(fasc::server::database::Database& db) : db_(db) {}

PurchaseOrderItemRowsResult PurchaseOrderItemController::list() const {
  static const std::vector<std::string> columns{"id", "purchase_order_id", "product_id", "quantity", "unit_price", "vat_rate", "currency"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.purchase_order_item", columns);
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
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.purchase_order_item", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return PurchaseOrderItemRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseOrderItemRowResult::success(
        fasc::server::controllers::dto::PurchaseOrderItemRowDto{rowToEntity(*row)});
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.purchaseOrderId));
  }
  if (!dto.productId.has_value()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productId));
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.quantity));
  }
  if (dto.unitPrice.has_value()) {
    columns.push_back("unit_price");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.unitPrice));
  }
  if (dto.vatRate.has_value()) {
    columns.push_back("vat_rate");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.vatRate));
  }
  if (dto.currency.has_value()) {
    columns.push_back("currency");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.currency));
  }
  if (columns.empty()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.purchase_order_item", columns, values);
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
    values.push_back(fasc::server::database::makeSqlParameter(*dto.purchaseOrderId));
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productId));
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.quantity));
  }
  if (dto.unitPrice.has_value()) {
    columns.push_back("unit_price");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.unitPrice));
  }
  if (dto.vatRate.has_value()) {
    columns.push_back("vat_rate");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.vatRate));
  }
  if (dto.currency.has_value()) {
    columns.push_back("currency");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.currency));
  }
  if (columns.empty()) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.purchase_order_item", columns, values, keys, keyValues(key));
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
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.purchase_order_item", keys, keyValues(key));
    });
    return PurchaseOrderItemMutationResult::success(
        fasc::server::controllers::dto::PurchaseOrderItemMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseOrderItemMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
