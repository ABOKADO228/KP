#pragma once

#include <persistence/FarmPlotProductionProduct.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_plot_production_product.
struct FarmPlotProductionProductKeyDto {
  /// Значение колонки product_id.
  int productId{};

  /// Значение колонки farm_plot_id.
  int farmPlotId{};

};

/// DTO создания строки таблицы farm_plot_production_product.
struct FarmPlotProductionProductCreateDto {
  /// Значение колонки product_id.
  std::optional<int> productId;

  /// Значение колонки farm_plot_id.
  std::optional<int> farmPlotId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки production_now.
  std::optional<int> productionNow;

};

/// DTO обновления строки таблицы farm_plot_production_product.
struct FarmPlotProductionProductUpdateDto {
  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки production_now.
  std::optional<int> productionNow;

};

/// DTO строки таблицы farm_plot_production_product.
struct FarmPlotProductionProductRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmPlotProductionProductEntity data;
};

/// DTO списка строк таблицы farm_plot_production_product.
struct FarmPlotProductionProductRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmPlotProductionProductEntity> rows;
};

/// DTO результата изменения таблицы farm_plot_production_product.
struct FarmPlotProductionProductMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
