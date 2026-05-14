#pragma once

#include <persistence/SupplierProductPrice.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы supplier_product_price.
struct SupplierProductPriceKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы supplier_product_price.
struct SupplierProductPriceCreateDto {
  /// Значение колонки supplier_id.
  std::optional<int> supplierId;

  /// Значение колонки product_id.
  std::optional<int> productId;

  /// Значение колонки purchase_price.
  std::optional<double> purchasePrice;

  /// Значение колонки valid_from.
  std::optional<std::string> validFrom;

  /// Значение колонки valid_until.
  std::optional<std::string> validUntil;

};

/// DTO обновления строки таблицы supplier_product_price.
struct SupplierProductPriceUpdateDto {
  /// Значение колонки supplier_id.
  std::optional<int> supplierId;

  /// Значение колонки product_id.
  std::optional<int> productId;

  /// Значение колонки purchase_price.
  std::optional<double> purchasePrice;

  /// Значение колонки valid_from.
  std::optional<std::string> validFrom;

  /// Значение колонки valid_until.
  std::optional<std::string> validUntil;

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
