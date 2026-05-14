#include <controllers/app/FarmAssociation.hpp>

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


fasc::server::persistence::FarmAssociationEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmAssociationEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "inn")) {
    entity.inn = *value;
  } else {
    entity.inn.reset();
  }
  if (const auto value = optionalColumn(row, "ogrn")) {
    entity.ogrn = *value;
  } else {
    entity.ogrn.reset();
  }
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
  if (const auto value = optionalColumn(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmAssociationController::FarmAssociationController(fasc::server::database::Database& db) : db_(db) {}

FarmAssociationRowsResult FarmAssociationController::list() const {
  static const std::vector<std::string> columns{"id", "inn", "ogrn", "name", "legal_address", "status"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_association", columns);
    });

    fasc::server::controllers::dto::FarmAssociationRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmAssociationRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmAssociationRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationRowResult FarmAssociationController::load(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "inn", "ogrn", "name", "legal_address", "status"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_association", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmAssociationRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmAssociationRowResult::success(
        fasc::server::controllers::dto::FarmAssociationRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmAssociationRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::create(
    const fasc::server::controllers::dto::FarmAssociationCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.inn.has_value()) {
    columns.push_back("inn");
    values.push_back(fasc::server::database::SqlParameter{*dto.inn, false});
  }
  if (dto.ogrn.has_value()) {
    columns.push_back("ogrn");
    values.push_back(fasc::server::database::SqlParameter{*dto.ogrn, false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalAddress, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (columns.empty()) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_association", columns, values);
    });
    return FarmAssociationMutationResult::success(
        fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::update(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key,
    const fasc::server::controllers::dto::FarmAssociationUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.inn.has_value()) {
    columns.push_back("inn");
    values.push_back(fasc::server::database::SqlParameter{*dto.inn, false});
  }
  if (dto.ogrn.has_value()) {
    columns.push_back("ogrn");
    values.push_back(fasc::server::database::SqlParameter{*dto.ogrn, false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalAddress, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (columns.empty()) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_association", columns, values, keys, keyValues(key));
    });
    return FarmAssociationMutationResult::success(
        fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmAssociationMutationResult FarmAssociationController::erase(
    const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_association", keys, keyValues(key));
    });
    return FarmAssociationMutationResult::success(
        fasc::server::controllers::dto::FarmAssociationMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmAssociationMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
