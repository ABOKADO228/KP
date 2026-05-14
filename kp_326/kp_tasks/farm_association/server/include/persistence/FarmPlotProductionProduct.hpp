#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы farm_plot_production_product.
struct FarmPlotProductionProductEntity {
  /// Значение колонки product_id.
  std::uint64_t productId{};

  /// Значение колонки farm_plot_id.
  std::uint64_t farmPlotId{};

  /// Значение колонки quantity.
  int quantity{};

  /// Значение колонки production_now.
  int productionNow{};

};

#pragma db object(FarmPlotProductionProductEntity) table("farm_plot_production_product")
#pragma db member(FarmPlotProductionProductEntity::productId) column("product_id")
#pragma db member(FarmPlotProductionProductEntity::farmPlotId) column("farm_plot_id")
#pragma db member(FarmPlotProductionProductEntity::quantity) column("quantity")
#pragma db member(FarmPlotProductionProductEntity::productionNow) column("production_now")

} // namespace fasc::server::persistence
