#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/SupplierProductPrice.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы supplier_product_price.
struct SupplierProductPriceKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы supplier_product_price.
struct SupplierProductPriceCreateDto {
  /// Значение колонки supplier_id.
  std::optional<std::uint64_t> supplierId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки purchase_price.
  std::optional<double> purchasePrice;

  /// Значение колонки valid_from.
  std::optional<fasc::server::domain::Date> validFrom;

  /// Значение колонки valid_until.
  std::optional<fasc::server::domain::Date> validUntil;

};

/// DTO обновления строки таблицы supplier_product_price.
struct SupplierProductPriceUpdateDto {
  /// Значение колонки supplier_id.
  std::optional<std::uint64_t> supplierId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки purchase_price.
  std::optional<double> purchasePrice;

  /// Значение колонки valid_from.
  std::optional<fasc::server::domain::Date> validFrom;

  /// Значение колонки valid_until.
  std::optional<fasc::server::domain::Date> validUntil;

};

/// DTO строки таблицы supplier_product_price.
struct SupplierProductPriceRowDto {
  /// Данные строки.
  fasc::server::persistence::SupplierProductPriceEntity data;
};

/// DTO списка строк таблицы supplier_product_price.
struct SupplierProductPriceRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::SupplierProductPriceEntity> rows;
};

/// DTO результата изменения таблицы supplier_product_price.
struct SupplierProductPriceMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
