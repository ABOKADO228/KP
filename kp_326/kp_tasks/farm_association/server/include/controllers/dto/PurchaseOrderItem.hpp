#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/PurchaseOrderItem.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы purchase_order_item.
struct PurchaseOrderItemKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы purchase_order_item.
struct PurchaseOrderItemCreateDto {
  /// Значение колонки purchase_order_id.
  std::optional<std::uint64_t> purchaseOrderId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки quantity.
  std::optional<double> quantity;

  /// Значение колонки unit_price.
  std::optional<double> unitPrice;

  /// Значение колонки vat_rate.
  std::optional<double> vatRate;

  /// Значение колонки currency.
  std::optional<fasc::server::domain::CurrencyCode> currency;

};

/// DTO обновления строки таблицы purchase_order_item.
struct PurchaseOrderItemUpdateDto {
  /// Значение колонки purchase_order_id.
  std::optional<std::uint64_t> purchaseOrderId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки quantity.
  std::optional<double> quantity;

  /// Значение колонки unit_price.
  std::optional<double> unitPrice;

  /// Значение колонки vat_rate.
  std::optional<double> vatRate;

  /// Значение колонки currency.
  std::optional<fasc::server::domain::CurrencyCode> currency;

};

/// DTO строки таблицы purchase_order_item.
struct PurchaseOrderItemRowDto {
  /// Данные строки.
  fasc::server::persistence::PurchaseOrderItemEntity data;
};

/// DTO списка строк таблицы purchase_order_item.
struct PurchaseOrderItemRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PurchaseOrderItemEntity> rows;
};

/// DTO результата изменения таблицы purchase_order_item.
struct PurchaseOrderItemMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
