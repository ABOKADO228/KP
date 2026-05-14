#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/SalesRequisition.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы sales_requisition.
struct SalesRequisitionKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы sales_requisition.
struct SalesRequisitionCreateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки price_per_unit.
  std::optional<double> pricePerUnit;

  /// Значение колонки offer_date.
  std::optional<fasc::server::domain::Date> offerDate;

  /// Значение колонки valid_until.
  std::optional<fasc::server::domain::Date> validUntil;

  /// Значение колонки status.
  std::optional<fasc::server::domain::SalesRequisitionStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы sales_requisition.
struct SalesRequisitionUpdateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки product_id.
  std::optional<std::uint64_t> productId;

  /// Значение колонки quantity.
  std::optional<int> quantity;

  /// Значение колонки price_per_unit.
  std::optional<double> pricePerUnit;

  /// Значение колонки offer_date.
  std::optional<fasc::server::domain::Date> offerDate;

  /// Значение колонки valid_until.
  std::optional<fasc::server::domain::Date> validUntil;

  /// Значение колонки status.
  std::optional<fasc::server::domain::SalesRequisitionStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO строки таблицы sales_requisition.
struct SalesRequisitionRowDto {
  /// Данные строки.
  fasc::server::persistence::SalesRequisitionEntity data;
};

/// DTO списка строк таблицы sales_requisition.
struct SalesRequisitionRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::SalesRequisitionEntity> rows;
};

/// DTO результата изменения таблицы sales_requisition.
struct SalesRequisitionMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
