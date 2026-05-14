#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationMemberRowView {
  std::uint64_t id{};
  std::uint64_t associationId{};
  std::uint64_t personId{};
  std::optional<std::string> membershipNumber{};
  fasc::server::domain::Date joinedDate{};
  std::optional<fasc::server::domain::AssociationMemberStatus> status{};
  std::optional<std::string> notes{};
};

struct AssociationMemberRowsView {
  std::vector<AssociationMemberRowView> rows{};
};

struct AssociationMemberMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
