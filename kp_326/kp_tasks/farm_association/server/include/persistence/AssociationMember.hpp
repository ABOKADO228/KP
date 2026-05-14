#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы association_member.
struct AssociationMemberEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки association_id.
  int associationId{};

  /// Значение колонки person_id.
  int personId{};

  /// Значение колонки membership_number.
  odb::nullable<std::string> membershipNumber;

  /// Значение колонки joined_date.
  std::string joinedDate;

  /// Значение колонки status.
  odb::nullable<std::string> status;

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
