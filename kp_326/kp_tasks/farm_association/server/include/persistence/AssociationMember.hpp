#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы association_member.
struct AssociationMemberEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки association_id.
  std::uint64_t associationId{};

  /// Значение колонки person_id.
  std::uint64_t personId{};

  /// Значение колонки membership_number.
  odb::nullable<std::string> membershipNumber;

  /// Значение колонки joined_date.
  fasc::server::domain::Date joinedDate;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::AssociationMemberStatus> status;

  /// Значение колонки notes.
  odb::nullable<std::string> notes;

};

#pragma db object(AssociationMemberEntity) table("association_member")
#pragma db member(AssociationMemberEntity::id) id auto column("id")
#pragma db member(AssociationMemberEntity::associationId) column("association_id")
#pragma db member(AssociationMemberEntity::personId) column("person_id")
#pragma db member(AssociationMemberEntity::membershipNumber) column("membership_number")
#pragma db member(AssociationMemberEntity::joinedDate) column("joined_date")
#pragma db member(AssociationMemberEntity::status) column("status")
#pragma db member(AssociationMemberEntity::notes) column("notes")

} // namespace fasc::server::persistence
