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

/// Сущность таблицы purchase_requisition.
struct PurchaseRequisitionEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки farm_id.
  std::uint64_t farmId{};

  /// Значение колонки product_id.
  std::uint64_t productId{};

  /// Значение колонки quantity.
  int quantity{};

  /// Значение колонки max_price_per_unit.
  odb::nullable<double> maxPricePerUnit;

  /// Значение колонки offer_date.
  odb::nullable<fasc::server::domain::Date> offerDate;

  /// Значение колонки required_date.
  odb::nullable<fasc::server::domain::Date> requiredDate;

  /// Значение колонки priority.
  odb::nullable<int> priority;

  /// Значение колонки valid_until.
  odb::nullable<fasc::server::domain::Date> validUntil;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::PurchaseRequisitionStatus> status;

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

} // namespace persistence
} // namespace server
} // namespace fasc
