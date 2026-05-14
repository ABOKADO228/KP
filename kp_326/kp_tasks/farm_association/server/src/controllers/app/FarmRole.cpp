#include <controllers/app/FarmRole.hpp>

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


fasc::server::persistence::FarmRoleEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmRoleEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
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
    const fasc::server::controllers::dto::FarmRoleKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmRoleController::FarmRoleController(fasc::server::database::Database& db) : db_(db) {}

FarmRoleRowsResult FarmRoleController::list() const {
  static const std::vector<std::string> columns{"id", "name", "description"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_role", columns);
    });

    fasc::server::controllers::dto::FarmRoleRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmRoleRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmRoleRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleRowResult FarmRoleController::load(const fasc::server::controllers::dto::FarmRoleKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "name", "description"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_role", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmRoleRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmRoleRowResult::success(
        fasc::server::controllers::dto::FarmRoleRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmRoleRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleMutationResult FarmRoleController::create(
    const fasc::server::controllers::dto::FarmRoleCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    columns.push_back("id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.id), false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::SqlParameter{*dto.description, false});
  }
  if (columns.empty()) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_role", columns, values);
    });
    return FarmRoleMutationResult::success(
        fasc::server::controllers::dto::FarmRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleMutationResult FarmRoleController::update(
    const fasc::server::controllers::dto::FarmRoleKeyDto& key,
    const fasc::server::controllers::dto::FarmRoleUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::SqlParameter{*dto.description, false});
  }
  if (columns.empty()) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_role", columns, values, keys, keyValues(key));
    });
    return FarmRoleMutationResult::success(
        fasc::server::controllers::dto::FarmRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRoleMutationResult FarmRoleController::erase(
    const fasc::server::controllers::dto::FarmRoleKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_role", keys, keyValues(key));
    });
    return FarmRoleMutationResult::success(
        fasc::server::controllers::dto::FarmRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
