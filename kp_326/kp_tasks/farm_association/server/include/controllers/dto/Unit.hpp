#pragma once

#include <persistence/Unit.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы unit.
struct UnitKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы unit.
struct UnitCreateDto {
  /// Значение колонки id.
  std::optional<int> id;

  /// Значение колонки code.
  std::optional<std::string> code;

  /// Значение колонки name.
  std::optional<std::string> name;

};

/// DTO обновления строки таблицы unit.
struct UnitUpdateDto {
  /// Значение колонки code.
  std::optional<std::string> code;

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
