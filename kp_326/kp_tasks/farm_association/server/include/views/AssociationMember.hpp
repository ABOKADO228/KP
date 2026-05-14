#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationMemberRowView {
  int id{};
  int associationId{};
  int personId{};
  std::optional<std::string> membershipNumber{};
  std::string joinedDate{};
  std::optional<std::string> status{};
  std::optional<std::string> notes{};
};

struct AssociationMemberRowsView {
  std::vector<AssociationMemberRowView> rows{};
};

struct AssociationMemberMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
