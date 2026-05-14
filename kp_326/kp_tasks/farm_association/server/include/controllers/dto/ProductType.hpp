#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/ProductType.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы product_type.
struct ProductTypeKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы product_type.
struct ProductTypeCreateDto {
  /// Значение колонки id.
  std::optional<std::uint64_t> id;

  /// Значение колонки parent_id.
  std::optional<std::uint64_t> parentId;

  /// Значение колонки sku.
  std::optional<std::string> sku;

  /// Значение колонки product_level.
  std::optional<int> productLevel;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO обновления строки таблицы product_type.
struct ProductTypeUpdateDto {
  /// Значение колонки parent_id.
  std::optional<std::uint64_t> parentId;

  /// Значение колонки sku.
  std::optional<std::string> sku;

  /// Значение колонки product_level.
  std::optional<int> productLevel;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO строки таблицы product_type.
struct ProductTypeRowDto {
  /// Данные строки.
  fasc::server::persistence::ProductTypeEntity data;
};

/// DTO списка строк таблицы product_type.
struct ProductTypeRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::ProductTypeEntity> rows;
};

/// DTO результата изменения таблицы product_type.
struct ProductTypeMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
