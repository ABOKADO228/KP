#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <persistence/Person.hpp>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::controllers::dto {

/// Ключ строки таблицы person.
struct PersonKeyDto {
  /// Значение колонки id.
  std::uint64_t id{};

};

/// DTO создания строки таблицы person.
struct PersonCreateDto {
  /// Значение колонки first_name.
  std::optional<std::string> firstName;

  /// Значение колонки last_name.
  std::optional<std::string> lastName;

  /// Значение колонки middle_name.
  std::optional<std::string> middleName;

  /// Значение колонки birth_date.
  std::optional<fasc::server::domain::Date> birthDate;

  /// Значение колонки phone.
  std::optional<std::string> phone;

  /// Значение колонки email.
  std::optional<std::string> email;

  /// Значение колонки address.
  std::optional<std::string> address;

};

/// DTO обновления строки таблицы person.
struct PersonUpdateDto {
  /// Значение колонки first_name.
  std::optional<std::string> firstName;

  /// Значение колонки last_name.
  std::optional<std::string> lastName;

  /// Значение колонки middle_name.
  std::optional<std::string> middleName;

  /// Значение колонки birth_date.
  std::optional<fasc::server::domain::Date> birthDate;

  /// Значение колонки phone.
  std::optional<std::string> phone;

  /// Значение колонки email.
  std::optional<std::string> email;

  /// Значение колонки address.
  std::optional<std::string> address;

};

/// DTO строки таблицы person.
struct PersonRowDto {
  /// Данные строки.
  fasc::server::persistence::PersonEntity data;
};

/// DTO списка строк таблицы person.
struct PersonRowsDto {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::PersonEntity> rows;
};

/// DTO результата изменения таблицы person.
struct PersonMutationDto {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::controllers::dto
