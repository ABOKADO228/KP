#pragma once

#include <persistence/Contract.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы contract.
struct ContractKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы contract.
struct ContractCreateDto {
  /// Значение колонки supplier_id.
  std::optional<int> supplierId;

  /// Значение колонки farm_id.
  std::optional<int> farmId;

  /// Значение колонки association_id.
  std::optional<int> associationId;

  /// Значение колонки contract_number.
  std::optional<std::string> contractNumber;

  /// Значение колонки sign_date.
  std::optional<std::string> signDate;

  /// Значение колонки start_date.
  std::optional<std::string> startDate;

  /// Значение колонки end_date.
  std::optional<std::string> endDate;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO обновления строки таблицы contract.
struct ContractUpdateDto {
  /// Значение колонки supplier_id.
  std::optional<int> supplierId;

  /// Значение колонки farm_id.
  std::optional<int> farmId;

  /// Значение колонки association_id.
  std::optional<int> associationId;

  /// Значение колонки contract_number.
  std::optional<std::string> contractNumber;

  /// Значение колонки sign_date.
  std::optional<std::string> signDate;

  /// Значение колонки start_date.
  std::optional<std::string> startDate;

  /// Значение колонки end_date.
  std::optional<std::string> endDate;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO строки таблицы contract.
struct ContractRowDto {
  /// Данные строки.
  fasc::server::persistence::ContractEntity data;
};

/// DTO списка строк таблицы contract.
struct ContractRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::ContractEntity> rows;
};

/// DTO результата изменения таблицы contract.
struct ContractMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
