#include <controllers/app/SupplierProductPrice.hpp>

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


fasc::server::persistence::SupplierProductPriceEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::SupplierProductPriceEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  entity.supplierId = fasc::server::database::requireColumn<std::uint64_t>(row, "supplier_id");
  entity.productId = fasc::server::database::requireColumn<std::uint64_t>(row, "product_id");
  if (const auto value = fasc::server::database::optionalColumn<double>(row, "purchase_price")) {
    entity.purchasePrice = *value;
  } else {
    entity.purchasePrice.reset();
  }
  entity.validFrom = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "valid_from");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::Date>(row, "valid_until")) {
    entity.validUntil = *value;
  } else {
    entity.validUntil.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

SupplierProductPriceController::SupplierProductPriceController(fasc::server::database::Database& db) : db_(db) {}

SupplierProductPriceRowsResult SupplierProductPriceController::list() const {
  static const std::vector<std::string> columns{"id", "supplier_id", "product_id", "purchase_price", "valid_from", "valid_until"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.supplier_product_price", columns);
    });

    fasc::server::controllers::dto::SupplierProductPriceRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return SupplierProductPriceRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return SupplierProductPriceRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceRowResult SupplierProductPriceController::load(const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "supplier_id", "product_id", "purchase_price", "valid_from", "valid_until"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.supplier_product_price", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return SupplierProductPriceRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return SupplierProductPriceRowResult::success(
        fasc::server::controllers::dto::SupplierProductPriceRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return SupplierProductPriceRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceMutationResult SupplierProductPriceController::create(
    const fasc::server::controllers::dto::SupplierProductPriceCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.supplierId.has_value()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: supplier_id"});
  }
  if (dto.supplierId.has_value()) {
    columns.push_back("supplier_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.supplierId));
  }
  if (!dto.productId.has_value()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productId));
  }
  if (dto.purchasePrice.has_value()) {
    columns.push_back("purchase_price");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.purchasePrice));
  }
  if (!dto.validFrom.has_value()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: valid_from"});
  }
  if (dto.validFrom.has_value()) {
    columns.push_back("valid_from");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.validFrom));
  }
  if (dto.validUntil.has_value()) {
    columns.push_back("valid_until");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.validUntil));
  }
  if (columns.empty()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.supplier_product_price", columns, values);
    });
    return SupplierProductPriceMutationResult::success(
        fasc::server::controllers::dto::SupplierProductPriceMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceMutationResult SupplierProductPriceController::update(
    const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key,
    const fasc::server::controllers::dto::SupplierProductPriceUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.supplierId.has_value()) {
    columns.push_back("supplier_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.supplierId));
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productId));
  }
  if (dto.purchasePrice.has_value()) {
    columns.push_back("purchase_price");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.purchasePrice));
  }
  if (dto.validFrom.has_value()) {
    columns.push_back("valid_from");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.validFrom));
  }
  if (dto.validUntil.has_value()) {
    columns.push_back("valid_until");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.validUntil));
  }
  if (columns.empty()) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.supplier_product_price", columns, values, keys, keyValues(key));
    });
    return SupplierProductPriceMutationResult::success(
        fasc::server::controllers::dto::SupplierProductPriceMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierProductPriceMutationResult SupplierProductPriceController::erase(
    const fasc::server::controllers::dto::SupplierProductPriceKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.supplier_product_price", keys, keyValues(key));
    });
    return SupplierProductPriceMutationResult::success(
        fasc::server::controllers::dto::SupplierProductPriceMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierProductPriceMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
