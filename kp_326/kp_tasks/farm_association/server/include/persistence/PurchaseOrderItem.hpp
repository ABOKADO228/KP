#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы purchase_order_item.
struct PurchaseOrderItemEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки purchase_order_id.
  int purchaseOrderId{};

  /// Значение колонки product_id.
  int productId{};

  /// Значение колонки quantity.
  odb::nullable<double> quantity;

  /// Значение колонки unit_price.
  odb::nullable<double> unitPrice;

  /// Значение колонки vat_rate.
  odb::nullable<double> vatRate;

  /// Значение колонки currency.
  odb::nullable<std::string> currency;

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
