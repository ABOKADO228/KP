#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы supplier_product_price.
struct SupplierProductPriceEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки supplier_id.
  std::uint64_t supplierId{};

  /// Значение колонки product_id.
  std::uint64_t productId{};

  /// Значение колонки purchase_price.
  odb::nullable<double> purchasePrice;

  /// Значение колонки valid_from.
  fasc::server::domain::Date validFrom;

  /// Значение колонки valid_until.
  odb::nullable<fasc::server::domain::Date> validUntil;

};

#pragma db object(SupplierProductPriceEntity) table("supplier_product_price")
#pragma db member(SupplierProductPriceEntity::id) id auto column("id")
#pragma db member(SupplierProductPriceEntity::supplierId) column("supplier_id")
#pragma db member(SupplierProductPriceEntity::productId) column("product_id")
#pragma db member(SupplierProductPriceEntity::purchasePrice) column("purchase_price")
#pragma db member(SupplierProductPriceEntity::validFrom) column("valid_from")
#pragma db member(SupplierProductPriceEntity::validUntil) column("valid_until")

} // namespace fasc::server::persistence
