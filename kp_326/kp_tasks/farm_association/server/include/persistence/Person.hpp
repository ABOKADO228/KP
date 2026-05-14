#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы person.
struct PersonEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки first_name.
  odb::nullable<std::string> firstName;

  /// Значение колонки last_name.
  odb::nullable<std::string> lastName;

  /// Значение колонки middle_name.
  odb::nullable<std::string> middleName;

  /// Значение колонки birth_date.
  odb::nullable<fasc::server::domain::Date> birthDate;

  /// Значение колонки phone.
  odb::nullable<std::string> phone;

  /// Значение колонки email.
  odb::nullable<std::string> email;

  /// Значение колонки address.
  odb::nullable<std::string> address;

};

#pragma db object(PersonEntity) table("person")
#pragma db member(PersonEntity::id) id auto column("id")
#pragma db member(PersonEntity::firstName) column("first_name")
#pragma db member(PersonEntity::lastName) column("last_name")
#pragma db member(PersonEntity::middleName) column("middle_name")
#pragma db member(PersonEntity::birthDate) column("birth_date")
#pragma db member(PersonEntity::phone) column("phone")
#pragma db member(PersonEntity::email) column("email")
#pragma db member(PersonEntity::address) column("address")

} // namespace fasc::server::persistence
