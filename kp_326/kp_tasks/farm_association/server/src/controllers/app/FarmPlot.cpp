#include <controllers/app/FarmPlot.hpp>

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


fasc::server::persistence::FarmPlotEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::FarmPlotEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "farm_plot_type_id")) {
    entity.farmPlotTypeId = std::stoi(*value);
  } else {
    entity.farmPlotTypeId.reset();
  }
  if (const auto value = optionalColumn(row, "name")) {
    entity.name = *value;
  } else {
    entity.name.reset();
  }
  if (const auto value = optionalColumn(row, "area")) {
    entity.area = std::stod(*value);
  } else {
    entity.area.reset();
  }
  if (const auto value = optionalColumn(row, "location")) {
    entity.location = *value;
  } else {
    entity.location.reset();
  }
  if (const auto value = optionalColumn(row, "cadastral_number")) {
    entity.cadastralNumber = *value;
  } else {
    entity.cadastralNumber.reset();
  }
  if (const auto value = optionalColumn(row, "status")) {
    entity.status = *value;
  } else {
    entity.status.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::FarmPlotKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

FarmPlotController::FarmPlotController(fasc::server::database::Database& db) : db_(db) {}

FarmPlotRowsResult FarmPlotController::list() const {
  static const std::vector<std::string> columns{"id", "farm_plot_type_id", "name", "area", "location", "cadastral_number", "status"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.farm_plot", columns);
    });

    fasc::server::controllers::dto::FarmPlotRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return FarmPlotRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return FarmPlotRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotRowResult FarmPlotController::load(const fasc::server::controllers::dto::FarmPlotKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "farm_plot_type_id", "name", "area", "location", "cadastral_number", "status"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.farm_plot", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return FarmPlotRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return FarmPlotRowResult::success(
        fasc::server::controllers::dto::FarmPlotRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return FarmPlotRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotMutationResult FarmPlotController::create(
    const fasc::server::controllers::dto::FarmPlotCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.farmPlotTypeId.has_value()) {
    columns.push_back("farm_plot_type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotTypeId), false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.area.has_value()) {
    columns.push_back("area");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.area), false});
  }
  if (dto.location.has_value()) {
    columns.push_back("location");
    values.push_back(fasc::server::database::SqlParameter{*dto.location, false});
  }
  if (dto.cadastralNumber.has_value()) {
    columns.push_back("cadastral_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.cadastralNumber, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (columns.empty()) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.farm_plot", columns, values);
    });
    return FarmPlotMutationResult::success(
        fasc::server::controllers::dto::FarmPlotMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotMutationResult FarmPlotController::update(
    const fasc::server::controllers::dto::FarmPlotKeyDto& key,
    const fasc::server::controllers::dto::FarmPlotUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.farmPlotTypeId.has_value()) {
    columns.push_back("farm_plot_type_id");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.farmPlotTypeId), false});
  }
  if (dto.name.has_value()) {
    columns.push_back("name");
    values.push_back(fasc::server::database::SqlParameter{*dto.name, false});
  }
  if (dto.area.has_value()) {
    columns.push_back("area");
    values.push_back(fasc::server::database::SqlParameter{std::to_string(*dto.area), false});
  }
  if (dto.location.has_value()) {
    columns.push_back("location");
    values.push_back(fasc::server::database::SqlParameter{*dto.location, false});
  }
  if (dto.cadastralNumber.has_value()) {
    columns.push_back("cadastral_number");
    values.push_back(fasc::server::database::SqlParameter{*dto.cadastralNumber, false});
  }
  if (dto.status.has_value()) {
    columns.push_back("status");
    values.push_back(fasc::server::database::SqlParameter{*dto.status, false});
  }
  if (columns.empty()) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.farm_plot", columns, values, keys, keyValues(key));
    });
    return FarmPlotMutationResult::success(
        fasc::server::controllers::dto::FarmPlotMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

FarmPlotMutationResult FarmPlotController::erase(
    const fasc::server::controllers::dto::FarmPlotKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.farm_plot", keys, keyValues(key));
    });
    return FarmPlotMutationResult::success(
        fasc::server::controllers::dto::FarmPlotMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return FarmPlotMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
