#include <controllers/app/Person.hpp>

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


fasc::server::persistence::PersonEntity rowToEntity(const fasc::server::database::SqlRow& row) {
  fasc::server::persistence::PersonEntity entity;
  entity.id = std::stoi(requireColumn(row, "id"));
  if (const auto value = optionalColumn(row, "first_name")) {
    entity.firstName = *value;
  } else {
    entity.firstName.reset();
  }
  if (const auto value = optionalColumn(row, "last_name")) {
    entity.lastName = *value;
  } else {
    entity.lastName.reset();
  }
  if (const auto value = optionalColumn(row, "middle_name")) {
    entity.middleName = *value;
  } else {
    entity.middleName.reset();
  }
  if (const auto value = optionalColumn(row, "birth_date")) {
    entity.birthDate = *value;
  } else {
    entity.birthDate.reset();
  }
  if (const auto value = optionalColumn(row, "phone")) {
    entity.phone = *value;
  } else {
    entity.phone.reset();
  }
  if (const auto value = optionalColumn(row, "email")) {
    entity.email = *value;
  } else {
    entity.email.reset();
  }
  if (const auto value = optionalColumn(row, "address")) {
    entity.address = *value;
  } else {
    entity.address.reset();
  }
  return entity;
}

std::vector<fasc::server::database::SqlParameter> keyValues(
    const fasc::server::controllers::dto::PersonKeyDto& key) {
  std::vector<fasc::server::database::SqlParameter> values;
  values.push_back(fasc::server::database::SqlParameter{std::to_string(key.id), false});
  return values;
}

} // namespace

PersonController::PersonController(fasc::server::database::Database& db) : db_(db) {}

PersonRowsResult PersonController::list() const {
  static const std::vector<std::string> columns{"id", "first_name", "last_name", "middle_name", "birth_date", "phone", "email", "address"};
  try {
    const auto rows = db_.invokeTransactionally([&] {
      return db_.selectRows("public.person", columns);
    });

    fasc::server::controllers::dto::PersonRowsDto dto;
    // Собираем строки ответа.
    for (const auto& row : rows) {
      dto.rows.push_back(rowToEntity(row));
    }
    return PersonRowsResult::success(std::move(dto));
  } catch (const std::exception& exception) {
    return PersonRowsResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonRowResult PersonController::load(const fasc::server::controllers::dto::PersonKeyDto& key) const {
  static const std::vector<std::string> columns{"id", "first_name", "last_name", "middle_name", "birth_date", "phone", "email", "address"};
  static const std::vector<std::string> keys{"id"};
  try {
    const auto row = db_.invokeTransactionally([&] {
      return db_.selectOneRow("public.person", columns, keys, keyValues(key));
    });
    if (!row.has_value()) {
      return PersonRowResult::failure(FarmEntityError{FarmEntityErrorCode::NotFound, "Row not found"});
    }
    return PersonRowResult::success(
        fasc::server::controllers::dto::PersonRowDto{rowToEntity(*row)});
  } catch (const std::exception& exception) {
    return PersonRowResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonMutationResult PersonController::create(
    const fasc::server::controllers::dto::PersonCreateDto& dto) const {
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.firstName.has_value()) {
    columns.push_back("first_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.firstName, false});
  }
  if (dto.lastName.has_value()) {
    columns.push_back("last_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.lastName, false});
  }
  if (dto.middleName.has_value()) {
    columns.push_back("middle_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.middleName, false});
  }
  if (dto.birthDate.has_value()) {
    columns.push_back("birth_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.birthDate, false});
  }
  if (dto.phone.has_value()) {
    columns.push_back("phone");
    values.push_back(fasc::server::database::SqlParameter{*dto.phone, false});
  }
  if (dto.email.has_value()) {
    columns.push_back("email");
    values.push_back(fasc::server::database::SqlParameter{*dto.email, false});
  }
  if (dto.address.has_value()) {
    columns.push_back("address");
    values.push_back(fasc::server::database::SqlParameter{*dto.address, false});
  }
  if (columns.empty()) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.insertRow("public.person", columns, values);
    });
    return PersonMutationResult::success(
        fasc::server::controllers::dto::PersonMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonMutationResult PersonController::update(
    const fasc::server::controllers::dto::PersonKeyDto& key,
    const fasc::server::controllers::dto::PersonUpdateDto& dto) const {
  static const std::vector<std::string> keys{"id"};
  std::vector<std::string> columns;
  std::vector<fasc::server::database::SqlParameter> values;
  if (dto.firstName.has_value()) {
    columns.push_back("first_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.firstName, false});
  }
  if (dto.lastName.has_value()) {
    columns.push_back("last_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.lastName, false});
  }
  if (dto.middleName.has_value()) {
    columns.push_back("middle_name");
    values.push_back(fasc::server::database::SqlParameter{*dto.middleName, false});
  }
  if (dto.birthDate.has_value()) {
    columns.push_back("birth_date");
    values.push_back(fasc::server::database::SqlParameter{*dto.birthDate, false});
  }
  if (dto.phone.has_value()) {
    columns.push_back("phone");
    values.push_back(fasc::server::database::SqlParameter{*dto.phone, false});
  }
  if (dto.email.has_value()) {
    columns.push_back("email");
    values.push_back(fasc::server::database::SqlParameter{*dto.email, false});
  }
  if (dto.address.has_value()) {
    columns.push_back("address");
    values.push_back(fasc::server::database::SqlParameter{*dto.address, false});
  }
  if (columns.empty()) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::InvalidInput, "No writable columns provided"});
  }

  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.updateRows("public.person", columns, values, keys, keyValues(key));
    });
    return PersonMutationResult::success(
        fasc::server::controllers::dto::PersonMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

PersonMutationResult PersonController::erase(
    const fasc::server::controllers::dto::PersonKeyDto& key) const {
  static const std::vector<std::string> keys{"id"};
  try {
    const unsigned long long affectedRows = db_.invokeTransactionally([&] {
      return db_.deleteRows("public.person", keys, keyValues(key));
    });
    return PersonMutationResult::success(
        fasc::server::controllers::dto::PersonMutationDto{affectedRows});
  } catch (const std::exception& exception) {
    return PersonMutationResult::failure(
        FarmEntityError{FarmEntityErrorCode::PersistenceFailure, exception.what()});
  }
}

} // namespace fasc::server::controllers::app
