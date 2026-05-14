#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/AssociationMember.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы association_member.
struct AssociationMemberKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы association_member.
struct AssociationMemberCreateDto {
  /// Значение колонки association_id.
  std::optional<std::uint64_t> associationId;

  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки membership_number.
  std::optional<std::string> membershipNumber;

  /// Значение колонки joined_date.
  std::optional<fasc::server::domain::Date> joinedDate;

  /// Значение колонки status.
  std::optional<fasc::server::domain::AssociationMemberStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO обновления строки таблицы association_member.
struct AssociationMemberUpdateDto {
  /// Значение колонки association_id.
  std::optional<std::uint64_t> associationId;

  /// Значение колонки person_id.
  std::optional<std::uint64_t> personId;

  /// Значение колонки membership_number.
  std::optional<std::string> membershipNumber;

  /// Значение колонки joined_date.
  std::optional<fasc::server::domain::Date> joinedDate;

  /// Значение колонки status.
  std::optional<fasc::server::domain::AssociationMemberStatus> status;

  /// Значение колонки notes.
  std::optional<std::string> notes;

};

/// DTO строки таблицы association_member.
struct AssociationMemberRowDto {
  /// Данные строки.
  fasc::server::persistence::AssociationMemberEntity data;
};

/// DTO списка строк таблицы association_member.
struct AssociationMemberRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationMemberEntity> rows;
};

/// DTO результата изменения таблицы association_member.
struct AssociationMemberMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
