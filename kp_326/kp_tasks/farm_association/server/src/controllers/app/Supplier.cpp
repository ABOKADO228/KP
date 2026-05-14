#include <controllers/app/Supplier.hpp>

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


fasc::server::persistence::SupplierEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::SupplierEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "legal_address")) {
    entity.legalAddress = *value;
  } else {
    entity.legalAddress.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::SupplierStatus>(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::SupplierKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

SupplierController::SupplierController(fasc::server::database::Database& db) : db_(db) {}

SupplierRowsResult SupplierController::list() const {
  static const std::vector<std::string> columns{"id", "name", "legal_address", "status"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.supplier", columns);
    });

    fasc::server::controllers::dto::SupplierRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return SupplierRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return SupplierRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierRowResult SupplierController::load(const fasc::server::controllers::dto::SupplierKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "name", "legal_address", "status"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.supplier", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return SupplierRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return SupplierRowResult::success(
        fasc::server::controllers::dto::SupplierRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return SupplierRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierMutationResult SupplierController::create(
    const fasc::server::controllers::dto::SupplierCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.legalAddress));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (columns.empty()) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.supplier", columns, values);
    });
    return SupplierMutationResult::success(
        fasc::server::controllers::dto::SupplierMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierMutationResult SupplierController::update(
    const fasc::server::controllers::dto::SupplierKeyDto& key,
    const fasc::server::controllers::dto::SupplierUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.legalAddress));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (columns.empty()) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.supplier", columns, values, keys, keyValues(key));
    });
    return SupplierMutationResult::success(
        fasc::server::controllers::dto::SupplierMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

SupplierMutationResult SupplierController::erase(
    const fasc::server::controllers::dto::SupplierKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.supplier", keys, keyValues(key));
    });
    return SupplierMutationResult::success(
        fasc::server::controllers::dto::SupplierMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return SupplierMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
