#pragma once

#include <persistence/PurchaseOrder.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы purchase_order.
struct PurchaseOrderKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы purchase_order.
struct PurchaseOrderCreateDto {
  /// Значение колонки association_id.
  std::optional<int> associationId;

  /// Значение колонки supplier_id.
  std::optional<int> supplierId;

  /// Значение колонки delivery_address.
  std::optional<std::string> deliveryAddress;

  /// Значение колонки order_date.
  std::optional<std::string> orderDate;

  /// Значение колонки expected_delivery_date.
  std::optional<std::string> expectedDeliveryDate;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки total_amount.
  std::optional<double> totalAmount;

  /// Значение колонки received_at.
  std::optional<std::string> receivedAt;

  /// Значение колонки created_by.
  std::optional<int> createdBy;

};

/// DTO обновления строки таблицы purchase_order.
struct PurchaseOrderUpdateDto {
  /// Значение колонки association_id.
  std::optional<int> associationId;

  /// Значение колонки supplier_id.
  std::optional<int> supplierId;

  /// Значение колонки delivery_address.
  std::optional<std::string> deliveryAddress;

  /// Значение колонки order_date.
  std::optional<std::string> orderDate;

  /// Значение колонки expected_delivery_date.
  std::optional<std::string> expectedDeliveryDate;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки total_amount.
  std::optional<double> totalAmount;

  /// Значение колонки received_at.
  std::optional<std::string> receivedAt;

  /// Значение колонки created_by.
  std::optional<int> createdBy;

};

/// DTO строки таблицы purchase_order.
struct PurchaseOrderRowDto {
  /// Данные строки.
  fasc::server::persistence::PurchaseOrderEntity data;
};

/// DTO списка строк таблицы purchase_order.
struct PurchaseOrderRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PurchaseOrderEntity> rows;
};

/// DTO результата изменения таблицы purchase_order.
struct PurchaseOrderMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
