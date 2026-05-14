#include <controllers/app/FarmPlotType.hpp>

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


fasc::server::persistence::FarmPlotTypeEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmPlotTypeEntity entity;
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
  entity.farmPlotLevel = std::stoi(requireColumn(row, "farm_plot_level"));
  if (const auto value = optionalColumn(row, "parent_id")) {
    entity.parentId = std::stoi(*value);
  } else {
    entity.parentId.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmPlotTypeController::FarmPlotTypeController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotTypeRowsResult FarmPlotTypeController::list() const {
  static const std::vector<std::string> columns{"id", "name", "description", "farm_plot_level", "parent_id"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_plot_type", columns);
    });

    fasc::server::controllers::dto::FarmPlotTypeRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmPlotTypeRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotTypeRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeRowResult FarmPlotTypeController::load(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "name", "description", "farm_plot_level", "parent_id"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_plot_type", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmPlotTypeRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotTypeRowResult::success(
        fasc::server::controllers::dto::FarmPlotTypeRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotTypeRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeMutationResult FarmPlotTypeController::create(
    const fasc::server::controllers::dto::FarmPlotTypeCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (!dto.id.has_value()) {
    return FarmPlotTypeMutationResult::failure(
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
  if (!dto.farmPlotLevel.has_value()) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "Missing field: farm_plot_level"});
  }
  if (dto.farmPlotLevel.has_value()) {
    columns.push_back("farm_plot_level");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotLevel), false});
  }
  if (dto.parentId.has_value()) {
    columns.push_back("parent_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.parentId), false});
  }
  if (columns.empty()) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_plot_type", columns, values);
    });
    return FarmPlotTypeMutationResult::success(
        fasc::server::controllers::dto::FarmPlotTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeMutationResult FarmPlotTypeController::update(
    const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotTypeUpdateDto& dto) const {
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
  if (dto.farmPlotLevel.has_value()) {
    columns.push_back("farm_plot_level");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotLevel), false});
  }
  if (dto.parentId.has_value()) {
    columns.push_back("parent_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.parentId), false});
  }
  if (columns.empty()) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_plot_type", columns, values, keys, keyValues(key));
    });
    return FarmPlotTypeMutationResult::success(
        fasc::server::controllers::dto::FarmPlotTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotTypeMutationResult FarmPlotTypeController::erase(
    const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_plot_type", keys, keyValues(key));
    });
    return FarmPlotTypeMutationResult::success(
        fasc::server::controllers::dto::FarmPlotTypeMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotTypeMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
