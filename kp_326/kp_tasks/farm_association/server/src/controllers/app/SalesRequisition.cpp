#include <controllers/app/SalesRequisition.hpp>

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


fasc::server::persistence::SalesRequisitionEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::SalesRequisitionEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  if (const auto value = fasc::server::database::optionalColumn<std::uint64_t>(row, "farm_id")) {
    entity.farmId = *value;
  } else {
    entity.farmId.reset();
  }
  entity.productId = fasc::server::database::requireColumn<std::uint64_t>(row, "product_id");
  entity.quantity = fasc::server::database::requireColumn<int>(row, "quantity");
  if (const auto value = fasc::server::database::optionalColumn<double>(row, "price_per_unit")) {
    entity.pricePerUnit = *value;
  } else {
    entity.pricePerUnit.reset();
  }
  entity.offerDate = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "offer_date");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::Date>(row, "valid_until")) {
    entity.validUntil = *value;
  } else {
    entity.validUntil.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::SalesRequisitionStatus>(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "notes")) {
    entity.notes = *value;
  } else {
    entity.notes.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

SalesRequisitionController::SalesRequisitionController(fasc::server::database::Database& db) : db_(db) {}

SalesRequisitionRowsResult SalesRequisitionController::list() const {
  static const std::vector<std::string> columns{"id", "farm_id", "product_id", "quantity", "price_per_unit", "offer_date", "valid_until", "status", "notes"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.sales_requisition", columns);
    });

    fasc::server::controllers::dto::SalesRequisitionRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return SalesRequisitionRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return SalesRequisitionRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionRowResult SalesRequisitionController::load(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "farm_id", "product_id", "quantity", "price_per_unit", "offer_date", "valid_until", "status", "notes"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.sales_requisition", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return SalesRequisitionRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return SalesRequisitionRowResult::success(
        fasc::server::controllers::dto::SalesRequisitionRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return SalesRequisitionRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionMutationResult SalesRequisitionController::create(
    const fasc::server::controllers::dto::SalesRequisitionCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmId));
  }
  if (!dto.productId.has_value()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productId));
  }
  if (!dto.quantity.has_value()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.quantity));
  }
  if (dto.pricePerUnit.has_value()) {
    columns.push_back("price_per_unit");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.pricePerUnit));
  }
  if (!dto.offerDate.has_value()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: offer_date"});
  }
  if (dto.offerDate.has_value()) {
    columns.push_back("offer_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.offerDate));
  }
  if (dto.validUntil.has_value()) {
    columns.push_back("valid_until");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.validUntil));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.sales_requisition", columns, values);
    });
    return SalesRequisitionMutationResult::success(
        fasc::server::controllers::dto::SalesRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionMutationResult SalesRequisitionController::update(
    const fasc::server::controllers::dto::SalesRequisitionKeyDto& key,
    const fasc::server::controllers::dto::SalesRequisitionUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmId));
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productId));
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.quantity));
  }
  if (dto.pricePerUnit.has_value()) {
    columns.push_back("price_per_unit");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.pricePerUnit));
  }
  if (dto.offerDate.has_value()) {
    columns.push_back("offer_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.offerDate));
  }
  if (dto.validUntil.has_value()) {
    columns.push_back("valid_until");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.validUntil));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.sales_requisition", columns, values, keys, keyValues(key));
    });
    return SalesRequisitionMutationResult::success(
        fasc::server::controllers::dto::SalesRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SalesRequisitionMutationResult SalesRequisitionController::erase(
    const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.sales_requisition", keys, keyValues(key));
    });
    return SalesRequisitionMutationResult::success(
        fasc::server::controllers::dto::SalesRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SalesRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
