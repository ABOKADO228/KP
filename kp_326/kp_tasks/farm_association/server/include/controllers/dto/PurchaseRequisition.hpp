#pragma once

#include <persistence/PurchaseRequisition.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы purchase_requisition.
struct PurchaseRequisitionKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы purchase_requisition.
struct PurchaseRequisitionCreateDto {
  /// Значение колонки farm_id.
  std::optional<int> farmId;

  /// Значение колонки product_id.
  std::optional<int> productId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки max_price_per_unit.
  std::optional<double> maxPricePerUnit;

  /// Значение колонки offer_date.
  std::optional<std::string> offerDate;

  /// Значение колонки required_date.
  std::optional<std::string> requiredDate;

  /// Значение колонки priority.
  std::optional<int> priority;

  /// Значение колонки valid_until.
  std::optional<std::string> validUntil;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы purchase_requisition.
struct PurchaseRequisitionUpdateDto {
  /// Значение колонки farm_id.
  std::optional<int> farmId;

  /// Значение колонки product_id.
  std::optional<int> productId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки max_price_per_unit.
  std::optional<double> maxPricePerUnit;

  /// Значение колонки offer_date.
  std::optional<std::string> offerDate;

  /// Значение колонки required_date.
  std::optional<std::string> requiredDate;

  /// Значение колонки priority.
  std::optional<int> priority;

  /// Значение колонки valid_until.
  std::optional<std::string> validUntil;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO строки таблицы purchase_requisition.
struct PurchaseRequisitionRowDto {
  /// Данные строки.
  fasc::server::persistence::PurchaseRequisitionEntity data;
};

/// DTO списка строк таблицы purchase_requisition.
struct PurchaseRequisitionRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PurchaseRequisitionEntity> rows;
};

/// DTO результата изменения таблицы purchase_requisition.
struct PurchaseRequisitionMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
