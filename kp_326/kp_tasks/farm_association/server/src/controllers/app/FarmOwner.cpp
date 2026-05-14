#include <controllers/app/FarmOwner.hpp>

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


fasc::server::persistence::FarmOwnerEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmOwnerEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  entity.personId = std::stoi(requireColumn(row, "person_id"));
  if (const auto value = optionalColumn(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "rating")) {
    entity.rating = std::stod(*value);
  } else {
    entity.rating.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmOwnerKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmOwnerController::FarmOwnerController(fasc::server::database::Database& db) : db_(db) {}

FarmOwnerRowsResult FarmOwnerController::list() const {
  static const std::vector<std::string> columns{"id", "person_id", "status", "rating"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_owner", columns);
    });

    fasc::server::controllers::dto::FarmOwnerRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmOwnerRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmOwnerRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerRowResult FarmOwnerController::load(const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "person_id", "status", "rating"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_owner", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmOwnerRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmOwnerRowResult::success(
        fasc::server::controllers::dto::FarmOwnerRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmOwnerRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerMutationResult FarmOwnerController::create(
    const fasc::server::controllers::dto::FarmOwnerCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.personId.has_value()) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: person_id"});
  }
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.rating.has_value()) {
    columns.push_back("rating");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.rating), false});
  }
  if (columns.empty()) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_owner", columns, values);
    });
    return FarmOwnerMutationResult::success(
        fasc::server::controllers::dto::FarmOwnerMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerMutationResult FarmOwnerController::update(
    const fasc::server::controllers::dto::FarmOwnerKeyDto& key,
    const fasc::server::controllers::dto::FarmOwnerUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.personId.has_value()) {
    columns.push_back("person_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.personId), false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (dto.rating.has_value()) {
    columns.push_back("rating");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.rating), false});
  }
  if (columns.empty()) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_owner", columns, values, keys, keyValues(key));
    });
    return FarmOwnerMutationResult::success(
        fasc::server::controllers::dto::FarmOwnerMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmOwnerMutationResult FarmOwnerController::erase(
    const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_owner", keys, keyValues(key));
    });
    return FarmOwnerMutationResult::success(
        fasc::server::controllers::dto::FarmOwnerMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmOwnerMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
