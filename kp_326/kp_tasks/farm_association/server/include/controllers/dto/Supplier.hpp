#pragma once

#include <persistence/Supplier.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы supplier.
struct SupplierKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы supplier.
struct SupplierCreateDto {
  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки legal_address.
  std::optional<std::string> legalAddress;

  /// Значение колонки status.
  std::optional<std::string> status;

};

/// DTO обновления строки таблицы supplier.
struct SupplierUpdateDto {
  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки legal_address.
  std::optional<std::string> legalAddress;

  /// Значение колонки status.
  std::optional<std::string> status;

};

/// DTO строки таблицы supplier.
struct SupplierRowDto {
  /// Данные строки.
  fasc::server::persistence::SupplierEntity data;
};

/// DTO списка строк таблицы supplier.
struct SupplierRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::SupplierEntity> rows;
};

/// DTO результата изменения таблицы supplier.
struct SupplierMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
