#include <controllers/app/Farm.hpp>

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


fasc::server::persistence::FarmEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "legal_name")) {
    entity.legalName = *value;
  } else {
    entity.legalName.reset();
  }
  if (const auto value = optionalColumn(row, "legal_address")) {
    entity.legalAddress = *value;
  } else {
    entity.legalAddress.reset();
  }
  if (const auto value = optionalColumn(row, "actual_address")) {
    entity.actualAddress = *value;
  } else {
    entity.actualAddress.reset();
  }
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
  if (const auto value = optionalColumn(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "farm_type")) {
    entity.farmType = *value;
  } else {
    entity.farmType.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmController::FarmController(fasc::server::database::Database& db) : db_(db) {}

FarmRowsResult FarmController::list() const {
  static const std::vector<std::string> columns{"id", "name", "legal_name", "legal_address", "actual_address", "inn", "ogrn", "status", "farm_type"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm", columns);
    });

    fasc::server::controllers::dto::FarmRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmRowResult FarmController::load(const fasc::server::controllers::dto::FarmKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "name", "legal_name", "legal_address", "actual_address", "inn", "ogrn", "status", "farm_type"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmRowResult::success(
        fasc::server::controllers::dto::FarmRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmMutationResult FarmController::create(
    const fasc::server::controllers::dto::FarmCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.legalName.has_value()) {
    columns.push_back("legal_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalName, false});
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalAddress, false});
  }
  if (dto.actualAddress.has_value()) {
    columns.push_back("actual_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.actualAddress, false});
  }
  if (dto.inn.has_value()) {
    columns.push_back("inn");
    values.push_back(fasc::server::database::SqlParameter{*dto.inn, false});
  }
  if (dto.ogrn.has_value()) {
    columns.push_back("ogrn");
    values.push_back(fasc::server::database::SqlParameter{*dto.ogrn, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.farmType.has_value()) {
    columns.push_back("farm_type");
    values.push_back(fasc::server::database::SqlParameter{*dto.farmType, false});
  }
  if (columns.empty()) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm", columns, values);
    });
    return FarmMutationResult::success(
        fasc::server::controllers::dto::FarmMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmMutationResult FarmController::update(
    const fasc::server::controllers::dto::FarmKeyDto& key,
    const fasc::server::controllers::dto::FarmUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.legalName.has_value()) {
    columns.push_back("legal_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalName, false});
  }
  if (dto.legalAddress.has_value()) {
    columns.push_back("legal_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.legalAddress, false});
  }
  if (dto.actualAddress.has_value()) {
    columns.push_back("actual_address");
    values.push_back(fasc::server::database::SqlParameter{*dto.actualAddress, false});
  }
  if (dto.inn.has_value()) {
    columns.push_back("inn");
    values.push_back(fasc::server::database::SqlParameter{*dto.inn, false});
  }
  if (dto.ogrn.has_value()) {
    columns.push_back("ogrn");
    values.push_back(fasc::server::database::SqlParameter{*dto.ogrn, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.farmType.has_value()) {
    columns.push_back("farm_type");
    values.push_back(fasc::server::database::SqlParameter{*dto.farmType, false});
  }
  if (columns.empty()) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm", columns, values, keys, keyValues(key));
    });
    return FarmMutationResult::success(
        fasc::server::controllers::dto::FarmMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmMutationResult FarmController::erase(
    const fasc::server::controllers::dto::FarmKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm", keys, keyValues(key));
    });
    return FarmMutationResult::success(
        fasc::server::controllers::dto::FarmMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
