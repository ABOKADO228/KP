#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы purchase_order_item.
struct PurchaseOrderItemEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки purchase_order_id.
  std::uint64_t purchaseOrderId{};

  /// Значение колонки product_id.
  std::uint64_t productId{};

  /// Значение колонки quantity.
  odb::nullable<double> quantity;

  /// Значение колонки unit_price.
  odb::nullable<double> unitPrice;

  /// Значение колонки vat_rate.
  odb::nullable<double> vatRate;

  /// Значение колонки currency.
  odb::nullable<fasc::server::domain::CurrencyCode> currency;

};

#pragma db object(PurchaseOrderItemEntity) table("purchase_order_item")
#pragma db member(PurchaseOrderItemEntity::id) id auto column("id")
#pragma db member(PurchaseOrderItemEntity::purchaseOrderId) column("purchase_order_id")
#pragma db member(PurchaseOrderItemEntity::productId) column("product_id")
#pragma db member(PurchaseOrderItemEntity::quantity) column("quantity")
#pragma db member(PurchaseOrderItemEntity::unitPrice) column("unit_price")
#pragma db member(PurchaseOrderItemEntity::vatRate) column("vat_rate")
#pragma db member(PurchaseOrderItemEntity::currency) column("currency")

} // namespace fasc::server::persistence
