#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы purchase_order.
struct PurchaseOrderEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки association_id.
  int associationId{};

  /// Значение колонки supplier_id.
  int supplierId{};

  /// Значение колонки delivery_address.
  odb::nullable<std::string> deliveryAddress;

  /// Значение колонки order_date.
  std::string orderDate;

  /// Значение колонки expected_delivery_date.
  odb::nullable<std::string> expectedDeliveryDate;

  /// Значение колонки status.
  odb::nullable<std::string> status;

  /// Значение колонки total_amount.
  odb::nullable<double> totalAmount;

  /// Значение колонки received_at.
  odb::nullable<std::string> receivedAt;

  /// Значение колонки created_by.
  odb::nullable<int> createdBy;

};

#pragma db object(PurchaseOrderEntity) table("purchase_order")
#pragma db member(PurchaseOrderEntity::id) id auto column("id")
#pragma db member(PurchaseOrderEntity::associationId) column("association_id")
#pragma db member(PurchaseOrderEntity::supplierId) column("supplier_id")
#pragma db member(PurchaseOrderEntity::deliveryAddress) column("delivery_address")
#pragma db member(PurchaseOrderEntity::orderDate) column("order_date")
#pragma db member(PurchaseOrderEntity::expectedDeliveryDate) column("expected_delivery_date")
#pragma db member(PurchaseOrderEntity::status) column("status")
#pragma db member(PurchaseOrderEntity::totalAmount) column("total_amount")
#pragma db member(PurchaseOrderEntity::receivedAt) column("received_at")
#pragma db member(PurchaseOrderEntity::createdBy) column("created_by")

} // namespace fasc::server::persistence
