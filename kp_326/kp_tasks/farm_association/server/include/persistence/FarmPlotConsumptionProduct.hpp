#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductEntity {
  /// Значение колонки product_id.
  std::uint64_t productId{};

  /// Значение колонки farm_plot_id.
  std::uint64_t farmPlotId{};

  /// Значение колонки quantity.
  int quantity{};

  /// Значение колонки consumption_now.
  int consumptionNow{};

};

#pragma db object(FarmPlotConsumptionProductEntity) table("farm_plot_consumption_product")
#pragma db member(FarmPlotConsumptionProductEntity::productId) column("product_id")
#pragma db member(FarmPlotConsumptionProductEntity::farmPlotId) column("farm_plot_id")
#pragma db member(FarmPlotConsumptionProductEntity::quantity) column("quantity")
#pragma db member(FarmPlotConsumptionProductEntity::consumptionNow) column("consumption_now")

} // namespace fasc::server::persistence
