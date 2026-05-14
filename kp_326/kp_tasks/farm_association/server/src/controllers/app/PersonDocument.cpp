#include <controllers/app/PersonDocument.hpp>

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


fasc::server::persistence::PersonDocumentEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::PersonDocumentEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  entity.personId = fasc::server::database::requireColumn<std::uint64_t>(row, "person_id");
  entity.documentTypeId = fasc::server::database::requireColumn<std::uint64_t>(row, "document_type_id");
  if (const auto value = optionalColumn(row, "document_number")) {
    entity.documentNumber = *value;
  } else {
    entity.documentNumber.reset();
  }
  if (const auto value = optionalColumn(row, "issued_by")) {
    entity.issuedBy = *value;
  } else {
    entity.issuedBy.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::Date>(row, "issued_date")) {
    entity.issuedDate = *value;
  } else {
    entity.issuedDate.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::Date>(row, "expiration_date")) {
    entity.expirationDate = *value;
  } else {
    entity.expirationDate.reset();
  }
  if (const auto value = fasc::server::database::optionalColumn<bool>(row, "is_primary")) {
    entity.isPrimary = *value;
  } else {
    entity.isPrimary.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

PersonDocumentController::PersonDocumentController(fasc::server::database::Database& db) : db_(db) {}

PersonDocumentRowsResult PersonDocumentController::list() const {
  static const std::vector<std::string> columns{"id", "person_id", "document_type_id", "document_number", "issued_by", "issued_date", "expiration_date", "is_primary"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.person_document", columns);
    });

    fasc::server::controllers::dto::PersonDocumentRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return PersonDocumentRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PersonDocumentRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentRowResult PersonDocumentController::load(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "person_id", "document_type_id", "document_number", "issued_by", "issued_date", "expiration_date", "is_primary"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.person_document", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return PersonDocumentRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PersonDocumentRowResult::success(
        fasc::server::controllers::dto::PersonDocumentRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return PersonDocumentRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::create(
    const fasc::server::controllers::dto::PersonDocumentCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.personId.has_value()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.personId));
  }
  if (!dto.documentTypeId.has_value()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: document_type_id"});
  }
  if (dto.documentTypeId.has_value()) {
    columns.push_back("document_type_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.documentTypeId));
  }
  if (dto.documentNumber.has_value()) {
    columns.push_back("document_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.documentNumber));
  }
  if (dto.issuedBy.has_value()) {
    columns.push_back("issued_by");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.issuedBy));
  }
  if (dto.issuedDate.has_value()) {
    columns.push_back("issued_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.issuedDate));
  }
  if (dto.expirationDate.has_value()) {
    columns.push_back("expiration_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.expirationDate));
  }
  if (dto.isPrimary.has_value()) {
    columns.push_back("is_primary");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.isPrimary));
  }
  if (columns.empty()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.person_document", columns, values);
    });
    return PersonDocumentMutationResult::success(
        fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::update(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key,
    const fasc::server::controllers::dto::PersonDocumentUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.personId));
  }
  if (dto.documentTypeId.has_value()) {
    columns.push_back("document_type_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.documentTypeId));
  }
  if (dto.documentNumber.has_value()) {
    columns.push_back("document_number");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.documentNumber));
  }
  if (dto.issuedBy.has_value()) {
    columns.push_back("issued_by");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.issuedBy));
  }
  if (dto.issuedDate.has_value()) {
    columns.push_back("issued_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.issuedDate));
  }
  if (dto.expirationDate.has_value()) {
    columns.push_back("expiration_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.expirationDate));
  }
  if (dto.isPrimary.has_value()) {
    columns.push_back("is_primary");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.isPrimary));
  }
  if (columns.empty()) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.person_document", columns, values, keys, keyValues(key));
    });
    return PersonDocumentMutationResult::success(
        fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonDocumentMutationResult PersonDocumentController::erase(
    const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.person_document", keys, keyValues(key));
    });
    return PersonDocumentMutationResult::success(
        fasc::server::controllers::dto::PersonDocumentMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonDocumentMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
