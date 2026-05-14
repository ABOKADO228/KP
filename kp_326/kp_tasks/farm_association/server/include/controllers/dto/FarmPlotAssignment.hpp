#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmPlotAssignment.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_plot_assignment.
struct FarmPlotAssignmentKeyDto {
  /// Значение колонки farm_id.
  std::uint64_t farmId{};

  /// Значение колонки farm_plot_id.
  std::uint64_t farmPlotId{};

};

/// DTO создания строки таблицы farm_plot_assignment.
struct FarmPlotAssignmentCreateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки farm_plot_id.
  std::optional<std::uint64_t> farmPlotId;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmPlotAssignmentStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы farm_plot_assignment.
struct FarmPlotAssignmentUpdateDto {
  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmPlotAssignmentStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO строки таблицы farm_plot_assignment.
struct FarmPlotAssignmentRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmPlotAssignmentEntity data;
};

/// DTO списка строк таблицы farm_plot_assignment.
struct FarmPlotAssignmentRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotAssignmentEntity> rows;
};

/// DTO результата изменения таблицы farm_plot_assignment.
struct FarmPlotAssignmentMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
