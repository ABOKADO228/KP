#include <controllers/app/PurchaseRequisition.hpp>

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


fasc::server::persistence::PurchaseRequisitionEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::PurchaseRequisitionEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.farmId = std::stoi(requireColumn(row, "farm_id"));
  entity.productId = std::stoi(requireColumn(row, "product_id"));
  entity.quantity = std::stoi(requireColumn(row, "quantity"));
  if (const auto value = optionalColumn(row, "max_price_per_unit")) {
    entity.maxPricePerUnit = std::stod(*value);
  } else {
    entity.maxPricePerUnit.reset();
  }
  if (const auto value = optionalColumn(row, "offer_date")) {
    entity.offerDate = *value;
  } else {
    entity.offerDate.reset();
  }
  if (const auto value = optionalColumn(row, "required_date")) {
    entity.requiredDate = *value;
  } else {
    entity.requiredDate.reset();
  }
  if (const auto value = optionalColumn(row, "priority")) {
    entity.priority = std::stoi(*value);
  } else {
    entity.priority.reset();
  }
  if (const auto value = optionalColumn(row, "valid_until")) {
    entity.validUntil = *value;
  } else {
    entity.validUntil.reset();
  }
  if (const auto value = optionalColumn(row, "status")) {
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
    const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

PurchaseRequisitionController::PurchaseRequisitionController(fasc::server::database::Database& db) : db_(db) {}

PurchaseRequisitionRowsResult PurchaseRequisitionController::list() const {
  static const std::vector<std::string> columns{"id", "farm_id", "product_id", "quantity", "max_price_per_unit", "offer_date", "required_date", "priority", "valid_until", "status", "notes"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.purchase_requisition", columns);
    });

    fasc::server::controllers::dto::PurchaseRequisitionRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return PurchaseRequisitionRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PurchaseRequisitionRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionRowResult PurchaseRequisitionController::load(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "farm_id", "product_id", "quantity", "max_price_per_unit", "offer_date", "required_date", "priority", "valid_until", "status", "notes"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.purchase_requisition", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return PurchaseRequisitionRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PurchaseRequisitionRowResult::success(
        fasc::server::controllers::dto::PurchaseRequisitionRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionMutationResult PurchaseRequisitionController::create(
    const fasc::server::controllers::dto::PurchaseRequisitionCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.farmId.has_value()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (!dto.productId.has_value()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_id"});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productId), false});
  }
  if (!dto.quantity.has_value()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: quantity"});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (dto.maxPricePerUnit.has_value()) {
    columns.push_back("max_price_per_unit");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.maxPricePerUnit), false});
  }
  if (dto.offerDate.has_value()) {
    columns.push_back("offer_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.offerDate, false});
  }
  if (dto.requiredDate.has_value()) {
    columns.push_back("required_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.requiredDate, false});
  }
  if (dto.priority.has_value()) {
    columns.push_back("priority");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.priority), false});
  }
  if (dto.validUntil.has_value()) {
    columns.push_back("valid_until");
    values.push_back(fasc::server::database::SqlParameter{*dto.validUntil, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.purchase_requisition", columns, values);
    });
    return PurchaseRequisitionMutationResult::success(
        fasc::server::controllers::dto::PurchaseRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionMutationResult PurchaseRequisitionController::update(
    const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key,
    const fasc::server::controllers::dto::PurchaseRequisitionUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmId), false});
  }
  if (dto.productId.has_value()) {
    columns.push_back("product_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.productId), false});
  }
  if (dto.quantity.has_value()) {
    columns.push_back("quantity");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.quantity), false});
  }
  if (dto.maxPricePerUnit.has_value()) {
    columns.push_back("max_price_per_unit");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.maxPricePerUnit), false});
  }
  if (dto.offerDate.has_value()) {
    columns.push_back("offer_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.offerDate, false});
  }
  if (dto.requiredDate.has_value()) {
    columns.push_back("required_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.requiredDate, false});
  }
  if (dto.priority.has_value()) {
    columns.push_back("priority");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.priority), false});
  }
  if (dto.validUntil.has_value()) {
    columns.push_back("valid_until");
    values.push_back(fasc::server::database::SqlParameter{*dto.validUntil, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::SqlParameter{*dto.notes, false});
  }
  if (columns.empty()) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.purchase_requisition", columns, values, keys, keyValues(key));
    });
    return PurchaseRequisitionMutationResult::success(
        fasc::server::controllers::dto::PurchaseRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PurchaseRequisitionMutationResult PurchaseRequisitionController::erase(
    const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.purchase_requisition", keys, keyValues(key));
    });
    return PurchaseRequisitionMutationResult::success(
        fasc::server::controllers::dto::PurchaseRequisitionMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PurchaseRequisitionMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
