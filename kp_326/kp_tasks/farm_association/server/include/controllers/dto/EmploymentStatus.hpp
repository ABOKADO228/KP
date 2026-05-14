#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/EmploymentStatus.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы employment_status.
struct EmploymentStatusKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы employment_status.
struct EmploymentStatusCreateDto {
  /// Значение колонки id.
  std::optional<std::uint64_t> id;

  /// Значение колонки name.
  std::optional<fasc::server::domain::EmploymentStatusCode> name;

};

/// DTO обновления строки таблицы employment_status.
struct EmploymentStatusUpdateDto {
  /// Значение колонки name.
  std::optional<fasc::server::domain::EmploymentStatusCode> name;

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
