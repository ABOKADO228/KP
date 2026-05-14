#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationEmployeeRowView {
  std::uint64_t id{};
  std::uint64_t personId{};
  std::uint64_t associationId{};
  std::uint64_t roleId{};
  std::uint64_t employmentStatusId{};
  fasc::server::domain::Date hireDate{};
  std::optional<fasc::server::domain::Date> dismissalDate{};
  std::optional<double> salary{};
  std::optional<std::string> contractNumber{};
};

struct AssociationEmployeeRowsView {
  std::vector<AssociationEmployeeRowView> rows{};
};

struct AssociationEmployeeMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
