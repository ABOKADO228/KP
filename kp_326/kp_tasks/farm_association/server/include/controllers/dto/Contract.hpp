#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/Contract.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы contract.
struct ContractKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы contract.
struct ContractCreateDto {
  /// Значение колонки supplier_id.
  std::optional<std::uint64_t> supplierId;

  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки association_id.
  std::optional<std::uint64_t> associationId;

  /// Значение колонки contract_number.
  std::optional<std::string> contractNumber;

  /// Значение колонки sign_date.
  std::optional<fasc::server::domain::Date> signDate;

  /// Значение колонки start_date.
  std::optional<fasc::server::domain::Date> startDate;

  /// Значение колонки end_date.
  std::optional<fasc::server::domain::Date> endDate;

  /// Значение колонки status.
  std::optional<fasc::server::domain::ContractStatus> status;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO обновления строки таблицы contract.
struct ContractUpdateDto {
  /// Значение колонки supplier_id.
  std::optional<std::uint64_t> supplierId;

  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки association_id.
  std::optional<std::uint64_t> associationId;

  /// Значение колонки contract_number.
  std::optional<std::string> contractNumber;

  /// Значение колонки sign_date.
  std::optional<fasc::server::domain::Date> signDate;

  /// Значение колонки start_date.
  std::optional<fasc::server::domain::Date> startDate;

  /// Значение колонки end_date.
  std::optional<fasc::server::domain::Date> endDate;

  /// Значение колонки status.
  std::optional<fasc::server::domain::ContractStatus> status;

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
