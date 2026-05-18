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

/// Сущность таблицы sales_requisition.
struct SalesRequisitionEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки farm_id.
  odb::nullable<unsigned long long> farmId;

  /// Значение колонки product_id.
  std::uint64_t productId{};

  /// Значение колонки quantity.
  int quantity{};

  /// Значение колонки price_per_unit.
  odb::nullable<double> pricePerUnit;

  /// Значение колонки offer_date.
  fasc::server::domain::Date offerDate;

  /// Значение колонки valid_until.
  odb::nullable<fasc::server::domain::Date> validUntil;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::SalesRequisitionStatus> status;

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

} // namespace persistence
} // namespace server
} // namespace fasc
