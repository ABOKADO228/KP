#include <controllers/app/Unit.hpp>

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


fasc::server::persistence::UnitEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::UnitEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::UnitCode>(row, "code")) {
    entity.code = *value;
  } else {
    entity.code.reset();
  }
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::UnitKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

UnitController::UnitController(fasc::server::database::Database& db) : db_(db) {}

UnitRowsResult UnitController::list() const {
  static const std::vector<std::string> columns{"id", "code", "name"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.unit", columns);
    });

    fasc::server::controllers::dto::UnitRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return UnitRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return UnitRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitRowResult UnitController::load(const fasc::server::controllers::dto::UnitKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "code", "name"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.unit", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return UnitRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return UnitRowResult::success(
        fasc::server::controllers::dto::UnitRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return UnitRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitMutationResult UnitController::create(
    const fasc::server::controllers::dto::UnitCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    columns.push_back("id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.id));
  }
  if (dto.code.has_value()) {
    columns.push_back("code");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.code));
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (columns.empty()) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.unit", columns, values);
    });
    return UnitMutationResult::success(
        fasc::server::controllers::dto::UnitMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitMutationResult UnitController::update(
    const fasc::server::controllers::dto::UnitKeyDto& key,
    const fasc::server::controllers::dto::UnitUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.code.has_value()) {
    columns.push_back("code");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.code));
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (columns.empty()) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.unit", columns, values, keys, keyValues(key));
    });
    return UnitMutationResult::success(
        fasc::server::controllers::dto::UnitMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

UnitMutationResult UnitController::erase(
    const fasc::server::controllers::dto::UnitKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.unit", keys, keyValues(key));
    });
    return UnitMutationResult::success(
        fasc::server::controllers::dto::UnitMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return UnitMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
