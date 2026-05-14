#include <controllers/app/AssociationRole.hpp>

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


fasc::server::persistence::AssociationRoleEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::AssociationRoleEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::AssociationRoleCode>(row, "code")) {
    entity.code = *value;
  } else {
    entity.code.reset();
  }
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
    const fasc::server::controllers::dto::AssociationRoleKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

AssociationRoleController::AssociationRoleController(fasc::server::database::Database& db) : db_(db) {}

AssociationRoleRowsResult AssociationRoleController::list() const {
  static const std::vector<std::string> columns{"id", "code", "name", "description"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.association_role", columns);
    });

    fasc::server::controllers::dto::AssociationRoleRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return AssociationRoleRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationRoleRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleRowResult AssociationRoleController::load(const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "code", "name", "description"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.association_role", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return AssociationRoleRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationRoleRowResult::success(
        fasc::server::controllers::dto::AssociationRoleRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return AssociationRoleRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleMutationResult AssociationRoleController::create(
    const fasc::server::controllers::dto::AssociationRoleCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return AssociationRoleMutationResult::failure(
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
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.description));
  }
  if (columns.empty()) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.association_role", columns, values);
    });
    return AssociationRoleMutationResult::success(
        fasc::server::controllers::dto::AssociationRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleMutationResult AssociationRoleController::update(
    const fasc::server::controllers::dto::AssociationRoleKeyDto& key,
    const fasc::server::controllers::dto::AssociationRoleUpdateDto& dto) const {
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
  if (dto.description.has_value()) {
    columns.push_back("description");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.description));
  }
  if (columns.empty()) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.association_role", columns, values, keys, keyValues(key));
    });
    return AssociationRoleMutationResult::success(
        fasc::server::controllers::dto::AssociationRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationRoleMutationResult AssociationRoleController::erase(
    const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.association_role", keys, keyValues(key));
    });
    return AssociationRoleMutationResult::success(
        fasc::server::controllers::dto::AssociationRoleMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationRoleMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
