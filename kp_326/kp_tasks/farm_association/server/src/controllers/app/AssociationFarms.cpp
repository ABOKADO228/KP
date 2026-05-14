#include <controllers/app/AssociationFarms.hpp>

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


fasc::server::persistence::AssociationFarmsEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::AssociationFarmsEntity entity;
  entity.farmId = fasc::server::database::requireColumn<std::uint64_t>(row, "farm_id");
  entity.associationId = fasc::server::database::requireColumn<std::uint64_t>(row, "association_id");
  entity.joinDate = fasc::server::database::requireColumn<fasc::server::domain::Date>(row, "join_date");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::AssociationFarmsStatus>(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  if (const auto value = optionalColumn(row, "notes")) {
    entity.notes = *value;
  } else {
    entity.notes.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.farmId));
  values.push_back(fasc::server::database::makeSqlParameter(key.associationId));
  return values;
}

} // namespace

AssociationFarmsController::AssociationFarmsController(fasc::server::database::Database& db) : db_(db) {}

AssociationFarmsRowsResult AssociationFarmsController::list() const {
  static const std::vector<std::string> columns{"farm_id", "association_id", "join_date", "status", "notes"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.association_farms", columns);
    });

    fasc::server::controllers::dto::AssociationFarmsRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return AssociationFarmsRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return AssociationFarmsRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsRowResult AssociationFarmsController::load(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
  static const std::vector<std::string> columns{"farm_id", "association_id", "join_date", "status", "notes"};
  static const std::vector<std::string> keys{"farm_id", "association_id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.association_farms", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return AssociationFarmsRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return AssociationFarmsRowResult::success(
        fasc::server::controllers::dto::AssociationFarmsRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return AssociationFarmsRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::create(
    const fasc::server::controllers::dto::AssociationFarmsCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.farmId.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_id"});
  }
  if (dto.farmId.has_value()) {
    columns.push_back("farm_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.farmId));
  }
  if (!dto.associationId.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: association_id"});
  }
  if (dto.associationId.has_value()) {
    columns.push_back("association_id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.associationId));
  }
  if (!dto.joinDate.has_value()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: join_date"});
  }
  if (dto.joinDate.has_value()) {
    columns.push_back("join_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.joinDate));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.association_farms", columns, values);
    });
    return AssociationFarmsMutationResult::success(
        fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::update(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key,
    const fasc::server::controllers::dto::AssociationFarmsUpdateDto& dto) const {
  static const std::vector<std::string> keys{"farm_id", "association_id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.joinDate.has_value()) {
    columns.push_back("join_date");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.joinDate));
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.status));
  }
  if (dto.notes.has_value()) {
    columns.push_back("notes");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.notes));
  }
  if (columns.empty()) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.association_farms", columns, values, keys, keyValues(key));
    });
    return AssociationFarmsMutationResult::success(
        fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

AssociationFarmsMutationResult AssociationFarmsController::erase(
    const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const {
  static const std::vector<std::string> keys{"farm_id", "association_id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.association_farms", keys, keyValues(key));
    });
    return AssociationFarmsMutationResult::success(
        fasc::server::controllers::dto::AssociationFarmsMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return AssociationFarmsMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
