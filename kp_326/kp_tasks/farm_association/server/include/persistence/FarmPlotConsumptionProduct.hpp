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

/// Составной ключ таблицы farm_plot_consumption_product.
struct FarmPlotConsumptionProductKey {
  FarmPlotConsumptionProductKey() = default;
  FarmPlotConsumptionProductKey(std::uint64_t productIdValue, std::uint64_t farmPlotIdValue)
      : productId(productIdValue), farmPlotId(farmPlotIdValue) {}

  std::uint64_t productId{};
  std::uint64_t farmPlotId{};
};

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

  FarmPlotConsumptionProductKey id() const {
    return FarmPlotConsumptionProductKey{productId, farmPlotId};
  }

  void id(const FarmPlotConsumptionProductKey& key) {
    productId = key.productId;
    farmPlotId = key.farmPlotId;
  }

};

#pragma db value(FarmPlotConsumptionProductKey)
#pragma db member(FarmPlotConsumptionProductKey::productId) column("product_id")
#pragma db member(FarmPlotConsumptionProductKey::farmPlotId) column("farm_plot_id")

#pragma db object(FarmPlotConsumptionProductEntity) table("farm_plot_consumption_product")
#pragma db member(FarmPlotConsumptionProductEntity::id) virtual(FarmPlotConsumptionProductKey) access(id) id column("")
#pragma db member(FarmPlotConsumptionProductEntity::productId) transient
#pragma db member(FarmPlotConsumptionProductEntity::farmPlotId) transient
#pragma db member(FarmPlotConsumptionProductEntity::quantity) column("quantity")
#pragma db member(FarmPlotConsumptionProductEntity::consumptionNow) column("consumption_now")

} // namespace persistence
} // namespace server
} // namespace fasc
