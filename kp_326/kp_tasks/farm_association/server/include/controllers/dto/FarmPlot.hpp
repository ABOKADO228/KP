#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmPlot.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_plot.
struct FarmPlotKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы farm_plot.
struct FarmPlotCreateDto {
  /// Значение колонки farm_plot_type_id.
  std::optional<std::uint64_t> farmPlotTypeId;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки area.
  std::optional<double> area;

  /// Значение колонки location.
  std::optional<std::string> location;

  /// Значение колонки cadastral_number.
  std::optional<std::string> cadastralNumber;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmPlotStatus> status;

};

/// DTO обновления строки таблицы farm_plot.
struct FarmPlotUpdateDto {
  /// Значение колонки farm_plot_type_id.
  std::optional<std::uint64_t> farmPlotTypeId;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки area.
  std::optional<double> area;

  /// Значение колонки location.
  std::optional<std::string> location;

  /// Значение колонки cadastral_number.
  std::optional<std::string> cadastralNumber;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmPlotStatus> status;

};

/// DTO строки таблицы farm_plot.
struct FarmPlotRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmPlotEntity data;
};

/// DTO списка строк таблицы farm_plot.
struct FarmPlotRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotEntity> rows;
};

/// DTO результата изменения таблицы farm_plot.
struct FarmPlotMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
