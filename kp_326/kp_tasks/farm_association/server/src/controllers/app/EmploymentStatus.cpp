#include <controllers/app/EmploymentStatus.hpp>

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


fasc::server::persistence::EmploymentStatusEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::EmploymentStatusEntity entity;
  entity.id = fasc::server::database::requireColumn<std::uint64_t>(row, "id");
  if (const auto value = fasc::server::database::optionalColumn<fasc::server::domain::EmploymentStatusCode>(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::makeSqlParameter(key.id));
  return values;
}

} // namespace

EmploymentStatusController::EmploymentStatusController(fasc::server::database::Database& db) : db_(db) {}

EmploymentStatusRowsResult EmploymentStatusController::list() const {
  static const std::vector<std::string> columns{"id", "name"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.employment_status", columns);
    });

    fasc::server::controllers::dto::EmploymentStatusRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return EmploymentStatusRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return EmploymentStatusRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusRowResult EmploymentStatusController::load(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "name"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.employment_status", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return EmploymentStatusRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return EmploymentStatusRowResult::success(
        fasc::server::controllers::dto::EmploymentStatusRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return EmploymentStatusRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusMutationResult EmploymentStatusController::create(
    const fasc::server::controllers::dto::EmploymentStatusCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: id"});
  }
  if (dto.id.has_value()) {
    columns.push_back("id");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.id));
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (columns.empty()) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.employment_status", columns, values);
    });
    return EmploymentStatusMutationResult::success(
        fasc::server::controllers::dto::EmploymentStatusMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusMutationResult EmploymentStatusController::update(
    const fasc::server::controllers::dto::EmploymentStatusKeyDto& key,
    const fasc::server::controllers::dto::EmploymentStatusUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::makeSqlParameter(*dto.name));
  }
  if (columns.empty()) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.employment_status", columns, values, keys, keyValues(key));
    });
    return EmploymentStatusMutationResult::success(
        fasc::server::controllers::dto::EmploymentStatusMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

EmploymentStatusMutationResult EmploymentStatusController::erase(
    const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.employment_status", keys, keyValues(key));
    });
    return EmploymentStatusMutationResult::success(
        fasc::server::controllers::dto::EmploymentStatusMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return EmploymentStatusMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
