#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы purchase_requisition.
struct PurchaseRequisitionEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки farm_id.
  int farmId{};

  /// Значение колонки product_id.
  int productId{};

  /// Значение колонки quantity.
  int quantity{};

  /// Значение колонки max_price_per_unit.
  odb::nullable<double> maxPricePerUnit;

  /// Значение колонки offer_date.
  odb::nullable<std::string> offerDate;

  /// Значение колонки required_date.
  odb::nullable<std::string> requiredDate;

  /// Значение колонки priority.
  odb::nullable<int> priority;

  /// Значение колонки valid_until.
  odb::nullable<std::string> validUntil;

  /// Значение колонки status.
  odb::nullable<std::string> status;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

};

#pragma db object(PurchaseRequisitionEntity) table("purchase_requisition")
#pragma db member(PurchaseRequisitionEntity::id) id auto column("id")
#pragma db member(PurchaseRequisitionEntity::farmId) column("farm_id")
#pragma db member(PurchaseRequisitionEntity::productId) column("product_id")
#pragma db member(PurchaseRequisitionEntity::quantity) column("quantity")
#pragma db member(PurchaseRequisitionEntity::maxPricePerUnit) column("max_price_per_unit")
#pragma db member(PurchaseRequisitionEntity::offerDate) column("offer_date")
#pragma db member(PurchaseRequisitionEntity::requiredDate) column("required_date")
#pragma db member(PurchaseRequisitionEntity::priority) column("priority")
#pragma db member(PurchaseRequisitionEntity::validUntil) column("valid_until")
#pragma db member(PurchaseRequisitionEntity::status) column("status")
#pragma db member(PurchaseRequisitionEntity::notes) column("notes")

} // namespace fasc::server::persistence
