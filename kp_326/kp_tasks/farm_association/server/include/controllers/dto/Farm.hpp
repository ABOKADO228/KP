#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/Farm.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm.
struct FarmKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы farm.
struct FarmCreateDto {
  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки legal_name.
  std::optional<std::string> legalName;

  /// Значение колонки legal_address.
  std::optional<std::string> legalAddress;

  /// Значение колонки actual_address.
  std::optional<std::string> actualAddress;

  /// Значение колонки inn.
  std::optional<std::string> inn;

  /// Значение колонки ogrn.
  std::optional<std::string> ogrn;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmStatus> status;

  /// Значение колонки farm_type.
  std::optional<fasc::server::domain::FarmType> farmType;

};

/// DTO обновления строки таблицы farm.
struct FarmUpdateDto {
  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки legal_name.
  std::optional<std::string> legalName;

  /// Значение колонки legal_address.
  std::optional<std::string> legalAddress;

  /// Значение колонки actual_address.
  std::optional<std::string> actualAddress;

  /// Значение колонки inn.
  std::optional<std::string> inn;

  /// Значение колонки ogrn.
  std::optional<std::string> ogrn;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmStatus> status;

  /// Значение колонки farm_type.
  std::optional<fasc::server::domain::FarmType> farmType;

};

/// DTO строки таблицы farm.
struct FarmRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmEntity data;
};

/// DTO списка строк таблицы farm.
struct FarmRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmEntity> rows;
};

/// DTO результата изменения таблицы farm.
struct FarmMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
