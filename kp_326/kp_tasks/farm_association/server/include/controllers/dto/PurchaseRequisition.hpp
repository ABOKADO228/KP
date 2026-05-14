#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/PurchaseRequisition.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы purchase_requisition.
struct PurchaseRequisitionKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы purchase_requisition.
struct PurchaseRequisitionCreateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки max_price_per_unit.
  std::optional<double> maxPricePerUnit;

  /// Значение колонки offer_date.
  std::optional<fasc::server::domain::Date> offerDate;

  /// Значение колонки required_date.
  std::optional<fasc::server::domain::Date> requiredDate;

  /// Значение колонки priority.
  std::optional<int> priority;

  /// Значение колонки valid_until.
  std::optional<fasc::server::domain::Date> validUntil;

  /// Значение колонки status.
  std::optional<fasc::server::domain::PurchaseRequisitionStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы purchase_requisition.
struct PurchaseRequisitionUpdateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки max_price_per_unit.
  std::optional<double> maxPricePerUnit;

  /// Значение колонки offer_date.
  std::optional<fasc::server::domain::Date> offerDate;

  /// Значение колонки required_date.
  std::optional<fasc::server::domain::Date> requiredDate;

  /// Значение колонки priority.
  std::optional<int> priority;

  /// Значение колонки valid_until.
  std::optional<fasc::server::domain::Date> validUntil;

  /// Значение колонки status.
  std::optional<fasc::server::domain::PurchaseRequisitionStatus> status;

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
