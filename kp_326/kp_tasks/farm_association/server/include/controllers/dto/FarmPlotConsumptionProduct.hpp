#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmPlotConsumptionProduct.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductKeyDto {
  /// Значение колонки product_id.
  std::uint64_t productId{};

  /// Значение колонки farm_plot_id.
  std::uint64_t farmPlotId{};

};

/// DTO создания строки таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductCreateDto {
  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки farm_plot_id.
  std::optional<std::uint64_t> farmPlotId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки consumption_now.
  std::optional<int> consumptionNow;

};

/// DTO обновления строки таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductUpdateDto {
  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки consumption_now.
  std::optional<int> consumptionNow;

};

/// DTO строки таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmPlotConsumptionProductEntity data;
};

/// DTO списка строк таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotConsumptionProductEntity> rows;
};

/// DTO результата изменения таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
