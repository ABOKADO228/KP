#pragma once

#include <persistence/FarmRole.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_role.
struct FarmRoleKeyDto {
  /// Значение колонки id.
  int id{};

};

/// DTO создания строки таблицы farm_role.
struct FarmRoleCreateDto {
  /// Значение колонки id.
  std::optional<int> id;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO обновления строки таблицы farm_role.
struct FarmRoleUpdateDto {
  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO строки таблицы farm_role.
struct FarmRoleRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmRoleEntity data;
};

/// DTO списка строк таблицы farm_role.
struct FarmRoleRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmRoleEntity> rows;
};

/// DTO результата изменения таблицы farm_role.
struct FarmRoleMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
