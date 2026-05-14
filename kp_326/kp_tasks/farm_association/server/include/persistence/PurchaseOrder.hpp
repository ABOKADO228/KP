#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы purchase_order.
struct PurchaseOrderEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки association_id.
  std::uint64_t associationId{};

  /// Значение колонки supplier_id.
  std::uint64_t supplierId{};

  /// Значение колонки delivery_address.
  odb::nullable<std::string> deliveryAddress;

  /// Значение колонки order_date.
  fasc::server::domain::Date orderDate;

  /// Значение колонки expected_delivery_date.
  odb::nullable<fasc::server::domain::Date> expectedDeliveryDate;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::PurchaseOrderStatus> status;

  /// Значение колонки total_amount.
  odb::nullable<double> totalAmount;

  /// Значение колонки received_at.
  odb::nullable<fasc::server::domain::Date> receivedAt;

  /// Значение колонки created_by.
  odb::nullable<std::uint64_t> createdBy;

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
