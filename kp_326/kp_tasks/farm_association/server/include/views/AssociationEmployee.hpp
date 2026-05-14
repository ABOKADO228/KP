#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct AssociationEmployeeRowView {
  int id{};
  int personId{};
  int associationId{};
  int roleId{};
  int employmentStatusId{};
  std::string hireDate{};
  std::optional<std::string> dismissalDate{};
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
