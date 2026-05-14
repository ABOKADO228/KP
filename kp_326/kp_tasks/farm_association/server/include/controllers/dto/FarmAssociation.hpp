#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/FarmAssociation.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы farm_association.
struct FarmAssociationKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы farm_association.
struct FarmAssociationCreateDto {
  /// Значение колонки inn.
  std::optional<std::string> inn;

  /// Значение колонки ogrn.
  std::optional<std::string> ogrn;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки legal_address.
  std::optional<std::string> legalAddress;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmAssociationStatus> status;

};

/// DTO обновления строки таблицы farm_association.
struct FarmAssociationUpdateDto {
  /// Значение колонки inn.
  std::optional<std::string> inn;

  /// Значение колонки ogrn.
  std::optional<std::string> ogrn;

  /// Значение колонки name.
  std::optional<std::string> name;

  /// Значение колонки legal_address.
  std::optional<std::string> legalAddress;

  /// Значение колонки status.
  std::optional<fasc::server::domain::FarmAssociationStatus> status;

};

/// DTO строки таблицы farm_association.
struct FarmAssociationRowDto {
  /// Данные строки.
  fasc::server::persistence::FarmAssociationEntity data;
};

/// DTO списка строк таблицы farm_association.
struct FarmAssociationRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::FarmAssociationEntity> rows;
};

/// DTO результата изменения таблицы farm_association.
struct FarmAssociationMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
