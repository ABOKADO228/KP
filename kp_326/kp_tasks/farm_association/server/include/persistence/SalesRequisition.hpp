#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы sales_requisition.
struct SalesRequisitionEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки farm_id.
  odb::nullable<int> farmId;

  /// Значение колонки product_id.
  int productId{};

  /// Значение колонки quantity.
  int quantity{};

  /// Значение колонки price_per_unit.
  odb::nullable<double> pricePerUnit;

  /// Значение колонки offer_date.
  std::string offerDate;

  /// Значение колонки valid_until.
  odb::nullable<std::string> validUntil;

  /// Значение колонки status.
  odb::nullable<std::string> status;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

};

#pragma db object(SalesRequisitionEntity) table("sales_requisition")
#pragma db member(SalesRequisitionEntity::id) id auto column("id")
#pragma db member(SalesRequisitionEntity::farmId) column("farm_id")
#pragma db member(SalesRequisitionEntity::productId) column("product_id")
#pragma db member(SalesRequisitionEntity::quantity) column("quantity")
#pragma db member(SalesRequisitionEntity::pricePerUnit) column("price_per_unit")
#pragma db member(SalesRequisitionEntity::offerDate) column("offer_date")
#pragma db member(SalesRequisitionEntity::validUntil) column("valid_until")
#pragma db member(SalesRequisitionEntity::status) column("status")
#pragma db member(SalesRequisitionEntity::notes) column("notes")

} // namespace fasc::server::persistence
