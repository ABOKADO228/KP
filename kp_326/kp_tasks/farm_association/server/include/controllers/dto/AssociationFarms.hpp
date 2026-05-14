#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/AssociationFarms.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы association_farms.
struct AssociationFarmsKeyDto {
  /// Значение колонки farm_id.
  std::uint64_t farmId{};

  /// Значение колонки association_id.
  std::uint64_t associationId{};

};

/// DTO создания строки таблицы association_farms.
struct AssociationFarmsCreateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки association_id.
  std::optional<std::uint64_t> associationId;

  /// Значение колонки join_date.
  std::optional<fasc::server::domain::Date> joinDate;

  /// Значение колонки status.
  std::optional<fasc::server::domain::AssociationFarmsStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы association_farms.
struct AssociationFarmsUpdateDto {
  /// Значение колонки join_date.
  std::optional<fasc::server::domain::Date> joinDate;

  /// Значение колонки status.
  std::optional<fasc::server::domain::AssociationFarmsStatus> status;

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
