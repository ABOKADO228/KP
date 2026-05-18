#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#ifndef ODB_COMPILER
#include <persistence/DomainValueTraits.hpp>
#endif

#include <string>

namespace fasc {
namespace server {
namespace persistence {

/// Составной ключ таблицы farm_plot_production_product.
struct FarmPlotProductionProductKey {
  FarmPlotProductionProductKey() = default;
  FarmPlotProductionProductKey(std::uint64_t productIdValue, std::uint64_t farmPlotIdValue)
      : productId(productIdValue), farmPlotId(farmPlotIdValue) {}

  std::uint64_t productId{};
  std::uint64_t farmPlotId{};
};

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

  FarmPlotProductionProductKey id() const {
    return FarmPlotProductionProductKey{productId, farmPlotId};
  }

  void id(const FarmPlotProductionProductKey& key) {
    productId = key.productId;
    farmPlotId = key.farmPlotId;
  }

};

#pragma db value(FarmPlotProductionProductKey)
#pragma db member(FarmPlotProductionProductKey::productId) column("product_id")
#pragma db member(FarmPlotProductionProductKey::farmPlotId) column("farm_plot_id")

#pragma db object(FarmPlotProductionProductEntity) table("farm_plot_production_product")
#pragma db member(FarmPlotProductionProductEntity::id) virtual(FarmPlotProductionProductKey) access(id) id column("")
#pragma db member(FarmPlotProductionProductEntity::productId) transient
#pragma db member(FarmPlotProductionProductEntity::farmPlotId) transient
#pragma db member(FarmPlotProductionProductEntity::quantity) column("quantity")
#pragma db member(FarmPlotProductionProductEntity::productionNow) column("production_now")

} // namespace persistence
} // namespace server
} // namespace fasc
