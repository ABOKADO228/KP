#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmPlotType.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_plot_type.
struct FarmPlotTypeKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы farm_plot_type.
struct FarmPlotTypeCreateDto {
  /// Значение колонки id.
  std::optional<std::uint64_t> id;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

  /// Значение колонки farm_plot_level.
  std::optional<int> farmPlotLevel;

  /// Значение колонки parent_id.
  std::optional<std::uint64_t> parentId;

};

/// DTO обновления строки таблицы farm_plot_type.
struct FarmPlotTypeUpdateDto {
  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

  /// Значение колонки farm_plot_level.
  std::optional<int> farmPlotLevel;

  /// Значение колонки parent_id.
  std::optional<std::uint64_t> parentId;

};

/// DTO строки таблицы farm_plot_type.
struct FarmPlotTypeRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmPlotTypeEntity data;
};

/// DTO списка строк таблицы farm_plot_type.
struct FarmPlotTypeRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotTypeEntity> rows;
};

/// DTO результата изменения таблицы farm_plot_type.
struct FarmPlotTypeMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
