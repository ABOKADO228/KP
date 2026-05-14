#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/Unit.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы unit.
struct UnitKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы unit.
struct UnitCreateDto {
  /// Значение колонки id.
  std::optional<std::uint64_t> id;

  /// Значение колонки code.
  std::optional<fasc::server::domain::UnitCode> code;

  /// Значение колонки name.
  std::optional<std::string> name;

};

/// DTO обновления строки таблицы unit.
struct UnitUpdateDto {
  /// Значение колонки code.
  std::optional<fasc::server::domain::UnitCode> code;

  /// Значение колонки name.
  std::optional<std::string> name;

};

/// DTO строки таблицы unit.
struct UnitRowDto {
  /// Данные строки.
  fasc::server::persistence::UnitEntity data;
};

/// DTO списка строк таблицы unit.
struct UnitRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::UnitEntity> rows;
};

/// DTO результата изменения таблицы unit.
struct UnitMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
