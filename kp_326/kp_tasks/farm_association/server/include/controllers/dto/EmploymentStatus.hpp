#pragma once

#include <persistence/EmploymentStatus.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы employment_status.
struct EmploymentStatusKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы employment_status.
struct EmploymentStatusCreateDto {
  /// Значение колонки id.
  std::optional<int> id;

  /// Значение колонки name.
  std::optional<std::string> name;

};

/// DTO обновления строки таблицы employment_status.
struct EmploymentStatusUpdateDto {
  /// Значение колонки name.
  std::optional<std::string> name;

};

/// DTO строки таблицы employment_status.
struct EmploymentStatusRowDto {
  /// Данные строки.
  fasc::server::persistence::EmploymentStatusEntity data;
};

/// DTO списка строк таблицы employment_status.
struct EmploymentStatusRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::EmploymentStatusEntity> rows;
};

/// DTO результата изменения таблицы employment_status.
struct EmploymentStatusMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
