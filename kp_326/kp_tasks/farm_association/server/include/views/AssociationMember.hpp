#pragma once

#include <persistence/AssociationMember.hpp>

#include <vector>

namespace fasc::server::views {

/// View строки таблицы association_member.
struct AssociationMemberRowView {
  /// Данные строки.
  fasc::server::persistence::AssociationMemberEntity data;
};

/// View списка строк таблицы association_member.
struct AssociationMemberRowsView {
  /// Строки таблицы.
  std::vector<fasc::server::persistence::AssociationMemberEntity> rows;
};

/// View результата изменения таблицы association_member.
struct AssociationMemberMutationView {
  /// Количество затронутых строк.
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
