#include <controllers/app/ProductType.hpp>

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


fasc::server::persistence::ProductTypeEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::ProductTypeEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  if (const auto value = fasc::server::database::optionalColumn<std::uint64_t>(row, "parent_id")) {
    entity.parentId = *value;
  } else {
    entity.parentId.reset();
  }
  if (const auto value = optionalColumn(row, "sku")) {
    entity.sku = *value;
  } else {
    entity.sku.reset();
  }
  entity.productLevel = fasc::server::database::requireColumn<int>(row, "product_level");
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "description")) {
    entity.description = *value;
  } else {
    entity.description.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

ProductTypeController::ProductTypeController(fasc::server::database::Database& db) : db_(db) {}

ProductTypeRowsResult ProductTypeController::list() const {
  static const std::vector<std::string> columns{"id", "parent_id", "sku", "product_level", "name", "description"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.product_type", columns);
    });

    fasc::server::controllers::dto::ProductTypeRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return ProductTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ProductTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeRowResult ProductTypeController::load(const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "parent_id", "sku", "product_level", "name", "description"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.product_type", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return ProductTypeRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ProductTypeRowResult::success(
        fasc::server::controllers::dto::ProductTypeRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return ProductTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::create(
    const fasc::server::controllers::dto::ProductTypeCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    columns.push_back("id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.id));
  }
  if (dto.parentId.has_value()) {
    columns.push_back("parent_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.parentId));
  }
  if (dto.sku.has_value()) {
    columns.push_back("sku");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.sku));
  }
  if (!dto.productLevel.has_value()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: product_level"});
  }
  if (dto.productLevel.has_value()) {
    columns.push_back("product_level");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productLevel));
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.description));
  }
  if (columns.empty()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.product_type", columns, values);
    });
    return ProductTypeMutationResult::success(
        fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::update(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key,
    const fasc::server::controllers::dto::ProductTypeUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.parentId.has_value()) {
    columns.push_back("parent_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.parentId));
  }
  if (dto.sku.has_value()) {
    columns.push_back("sku");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.sku));
  }
  if (dto.productLevel.has_value()) {
    columns.push_back("product_level");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.productLevel));
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.description));
  }
  if (columns.empty()) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.product_type", columns, values, keys, keyValues(key));
    });
    return ProductTypeMutationResult::success(
        fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductTypeMutationResult ProductTypeController::erase(
    const fasc::server::controllers::dto::ProductTypeKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.product_type", keys, keyValues(key));
    });
    return ProductTypeMutationResult::success(
        fasc::server::controllers::dto::ProductTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
