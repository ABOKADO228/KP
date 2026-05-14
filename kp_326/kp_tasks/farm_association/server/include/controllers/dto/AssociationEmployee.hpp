#pragma once

#include <persistence/AssociationEmployee.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы association_employee.
struct AssociationEmployeeKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы association_employee.
struct AssociationEmployeeCreateDto {
  /// Значение колонки person_id.
  std::optional<int> personId;

  /// Значение колонки association_id.
  std::optional<int> associationId;

  /// Значение колонки role_id.
  std::optional<int> roleId;

  /// Значение колонки employment_status_id.
  std::optional<int> employmentStatusId;

  /// Значение колонки hire_date.
  std::optional<std::string> hireDate;

  /// Значение колонки dismissal_date.
  std::optional<std::string> dismissalDate;

  /// Значение колонки salary.
  std::optional<double> salary;

  /// Значение колонки contract_number.
  std::optional<std::string> contractNumber;

};

/// DTO обновления строки таблицы association_employee.
struct AssociationEmployeeUpdateDto {
  /// Значение колонки person_id.
  std::optional<int> personId;

  /// Значение колонки association_id.
  std::optional<int> associationId;

  /// Значение колонки role_id.
  std::optional<int> roleId;

  /// Значение колонки employment_status_id.
  std::optional<int> employmentStatusId;

  /// Значение колонки hire_date.
  std::optional<std::string> hireDate;

  /// Значение колонки dismissal_date.
  std::optional<std::string> dismissalDate;

  /// Значение колонки salary.
  std::optional<double> salary;

  /// Значение колонки contract_number.
  std::optional<std::string> contractNumber;

};

/// DTO строки таблицы association_employee.
struct AssociationEmployeeRowDto {
  /// Данные строки.
  fasc::server::persistence::AssociationEmployeeEntity data;
};

/// DTO списка строк таблицы association_employee.
struct AssociationEmployeeRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationEmployeeEntity> rows;
};

/// DTO результата изменения таблицы association_employee.
struct AssociationEmployeeMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
