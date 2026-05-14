#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmOwnership.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_ownership.
struct FarmOwnershipKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы farm_ownership.
struct FarmOwnershipCreateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки farm_owner_id.
  std::optional<std::uint64_t> farmOwnerId;

  /// Значение колонки ownership_percentage.
  std::optional<double> ownershipPercentage;

  /// Значение колонки started_at.
  std::optional<fasc::server::domain::Date> startedAt;

  /// Значение колонки ended_at.
  std::optional<fasc::server::domain::Date> endedAt;

};

/// DTO обновления строки таблицы farm_ownership.
struct FarmOwnershipUpdateDto {
  /// Значение колонки farm_id.
  std::optional<std::uint64_t> farmId;

  /// Значение колонки farm_owner_id.
  std::optional<std::uint64_t> farmOwnerId;

  /// Значение колонки ownership_percentage.
  std::optional<double> ownershipPercentage;

  /// Значение колонки started_at.
  std::optional<fasc::server::domain::Date> startedAt;

  /// Значение колонки ended_at.
  std::optional<fasc::server::domain::Date> endedAt;

};

/// DTO строки таблицы farm_ownership.
struct FarmOwnershipRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmOwnershipEntity data;
};

/// DTO списка строк таблицы farm_ownership.
struct FarmOwnershipRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmOwnershipEntity> rows;
};

/// DTO результата изменения таблицы farm_ownership.
struct FarmOwnershipMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
