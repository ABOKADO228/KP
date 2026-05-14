#include <controllers/app/Product.hpp>

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


fasc::server::persistence::ProductEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::ProductEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.typeId = std::stoi(requireColumn(row, "type_id"));
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "unit_id")) {
    entity.unitId = std::stoi(*value);
  } else {
    entity.unitId.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::ProductKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

ProductController::ProductController(fasc::server::database::Database& db) : db_(db) {}

ProductRowsResult ProductController::list() const {
  static const std::vector<std::string> columns{"id", "type_id", "name", "unit_id"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.product", columns);
    });

    fasc::server::controllers::dto::ProductRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return ProductRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return ProductRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductRowResult ProductController::load(const fasc::server::controllers::dto::ProductKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "type_id", "name", "unit_id"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.product", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return ProductRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return ProductRowResult::success(
        fasc::server::controllers::dto::ProductRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return ProductRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::create(
    const fasc::server::controllers::dto::ProductCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.typeId.has_value()) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: type_id"});
  }
  if (dto.typeId.has_value()) {
    columns.push_back("type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.typeId), false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.unitId.has_value()) {
    columns.push_back("unit_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.unitId), false});
  }
  if (columns.empty()) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.product", columns, values);
    });
    return ProductMutationResult::success(
        fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::update(
    const fasc::server::controllers::dto::ProductKeyDto& key,
    const fasc::server::controllers::dto::ProductUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.typeId.has_value()) {
    columns.push_back("type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.typeId), false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.unitId.has_value()) {
    columns.push_back("unit_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.unitId), false});
  }
  if (columns.empty()) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.product", columns, values, keys, keyValues(key));
    });
    return ProductMutationResult::success(
        fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

ProductMutationResult ProductController::erase(
    const fasc::server::controllers::dto::ProductKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.product", keys, keyValues(key));
    });
    return ProductMutationResult::success(
        fasc::server::controllers::dto::ProductMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return ProductMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
