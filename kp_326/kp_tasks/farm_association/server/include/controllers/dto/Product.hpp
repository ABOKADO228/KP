#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/Product.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы product.
struct ProductKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы product.
struct ProductCreateDto {
  /// Значение колонки type_id.
  std::optional<std::uint64_t> typeId;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки unit_id.
  std::optional<std::uint64_t> unitId;

};

/// DTO обновления строки таблицы product.
struct ProductUpdateDto {
  /// Значение колонки type_id.
  std::optional<std::uint64_t> typeId;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки unit_id.
  std::optional<std::uint64_t> unitId;

};

/// DTO строки таблицы product.
struct ProductRowDto {
  /// Данные строки.
  fasc::server::persistence::ProductEntity data;
};

/// DTO списка строк таблицы product.
struct ProductRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::ProductEntity> rows;
};

/// DTO результата изменения таблицы product.
struct ProductMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
