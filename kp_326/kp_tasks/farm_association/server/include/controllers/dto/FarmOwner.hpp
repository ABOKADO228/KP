#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmOwner.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_owner.
struct FarmOwnerKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы farm_owner.
struct FarmOwnerCreateDto {
  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmOwnerStatus> status;

  /// Значение колонки rating.
  std::optional<double> rating;

};

/// DTO обновления строки таблицы farm_owner.
struct FarmOwnerUpdateDto {
  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmOwnerStatus> status;

  /// Значение колонки rating.
  std::optional<double> rating;

};

/// DTO строки таблицы farm_owner.
struct FarmOwnerRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmOwnerEntity data;
};

/// DTO списка строк таблицы farm_owner.
struct FarmOwnerRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmOwnerEntity> rows;
};

/// DTO результата изменения таблицы farm_owner.
struct FarmOwnerMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
