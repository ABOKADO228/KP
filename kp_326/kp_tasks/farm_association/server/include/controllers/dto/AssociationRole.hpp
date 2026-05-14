#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/AssociationRole.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы association_role.
struct AssociationRoleKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы association_role.
struct AssociationRoleCreateDto {
  /// Значение колонки id.
  std::optional<std::uint64_t> id;

  /// Значение колонки code.
  std::optional<fasc::server::domain::AssociationRoleCode> code;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO обновления строки таблицы association_role.
struct AssociationRoleUpdateDto {
  /// Значение колонки code.
  std::optional<fasc::server::domain::AssociationRoleCode> code;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки description.
  std::optional<std::string> description;

};

/// DTO строки таблицы association_role.
struct AssociationRoleRowDto {
  /// Данные строки.
  fasc::server::persistence::AssociationRoleEntity data;
};

/// DTO списка строк таблицы association_role.
struct AssociationRoleRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationRoleEntity> rows;
};

/// DTO результата изменения таблицы association_role.
struct AssociationRoleMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
