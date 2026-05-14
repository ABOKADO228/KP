#include <controllers/app/IdentityDocumentType.hpp>

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


fasc::server::persistence::IdentityDocumentTypeEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::IdentityDocumentTypeEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "code")) {
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
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

IdentityDocumentTypeController::IdentityDocumentTypeController(fasc::server::database::Database& db) : db_(db) {}

IdentityDocumentTypeRowsResult IdentityDocumentTypeController::list() const {
  static const std::vector<std::string> columns{"id", "code", "name", "description"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.identity_document_type", columns);
    });

    fasc::server::controllers::dto::IdentityDocumentTypeRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return IdentityDocumentTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeRowResult IdentityDocumentTypeController::load(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "code", "name", "description"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.identity_document_type", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return IdentityDocumentTypeRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return IdentityDocumentTypeRowResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::create(
    const fasc::server::controllers::dto::IdentityDocumentTypeCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    columns.push_back("id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.id), false});
  }
  if (dto.code.has_value()) {
    columns.push_back("code");
    values.push_back(fasc::server::database::SqlParameter{*dto.code, false});
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
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.identity_document_type", columns, values);
    });
    return IdentityDocumentTypeMutationResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::update(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key,
    const fasc::server::controllers::dto::IdentityDocumentTypeUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.code.has_value()) {
    columns.push_back("code");
    values.push_back(fasc::server::database::SqlParameter{*dto.code, false});
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
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.identity_document_type", columns, values, keys, keyValues(key));
    });
    return IdentityDocumentTypeMutationResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

IdentityDocumentTypeMutationResult IdentityDocumentTypeController::erase(
    const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.identity_document_type", keys, keyValues(key));
    });
    return IdentityDocumentTypeMutationResult::success(
        fasc::server::controllers::dto::IdentityDocumentTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return IdentityDocumentTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
