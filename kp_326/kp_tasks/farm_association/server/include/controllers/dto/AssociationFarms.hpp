#pragma once

#include <persistence/AssociationFarms.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы association_farms.
struct AssociationFarmsKeyDto {
  /// Значение колонки farm_id.
  int farmId{};

  /// Значение колонки association_id.
  int associationId{};

};

/// DTO создания строки таблицы association_farms.
struct AssociationFarmsCreateDto {
  /// Значение колонки farm_id.
  std::optional<int> farmId;

  /// Значение колонки association_id.
  std::optional<int> associationId;

  /// Значение колонки join_date.
  std::optional<std::string> joinDate;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы association_farms.
struct AssociationFarmsUpdateDto {
  /// Значение колонки join_date.
  std::optional<std::string> joinDate;

  /// Значение колонки status.
  std::optional<std::string> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO строки таблицы association_farms.
struct AssociationFarmsRowDto {
  /// Данные строки.
  fasc::server::persistence::AssociationFarmsEntity data;
};

/// DTO списка строк таблицы association_farms.
struct AssociationFarmsRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationFarmsEntity> rows;
};

/// DTO результата изменения таблицы association_farms.
struct AssociationFarmsMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
