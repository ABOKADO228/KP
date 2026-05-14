#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmEmployee.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_employee.
struct FarmEmployeeKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы farm_employee.
struct FarmEmployeeCreateDto {
  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки role_id.
  std::optional<std::uint64_t> roleId;

  /// Значение колонки employment_status_id.
  std::optional<std::uint64_t> employmentStatusId;

  /// Значение колонки hire_date.
  std::optional<fasc::server::domain::Date> hireDate;

  /// Значение колонки dismissal_date.
  std::optional<fasc::server::domain::Date> dismissalDate;

  /// Значение колонки salary.
  std::optional<double> salary;

  /// Значение колонки employment_contract_number.
  std::optional<std::string> employmentContractNumber;

  /// Значение колонки is_primary_workplace.
  std::optional<bool> isPrimaryWorkplace;

};

/// DTO обновления строки таблицы farm_employee.
struct FarmEmployeeUpdateDto {
  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки role_id.
  std::optional<std::uint64_t> roleId;

  /// Значение колонки employment_status_id.
  std::optional<std::uint64_t> employmentStatusId;

  /// Значение колонки hire_date.
  std::optional<fasc::server::domain::Date> hireDate;

  /// Значение колонки dismissal_date.
  std::optional<fasc::server::domain::Date> dismissalDate;

  /// Значение колонки salary.
  std::optional<double> salary;

  /// Значение колонки employment_contract_number.
  std::optional<std::string> employmentContractNumber;

  /// Значение колонки is_primary_workplace.
  std::optional<bool> isPrimaryWorkplace;

};

/// DTO строки таблицы farm_employee.
struct FarmEmployeeRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmEmployeeEntity data;
};

/// DTO списка строк таблицы farm_employee.
struct FarmEmployeeRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmEmployeeEntity> rows;
};

/// DTO результата изменения таблицы farm_employee.
struct FarmEmployeeMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
