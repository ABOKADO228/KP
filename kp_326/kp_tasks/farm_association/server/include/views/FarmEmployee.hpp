#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmEmployeeRowView {
  int id{};
  int personId{};
  int farmId{};
  int roleId{};
  int employmentStatusId{};
  std::string hireDate{};
  std::optional<std::string> dismissalDate{};
  std::optional<double> salary{};
  std::optional<std::string> employmentContractNumber{};
  std::optional<int> isPrimaryWorkplace{};
};

struct FarmEmployeeRowsView {
  std::vector<FarmEmployeeRowView> rows{};
};

struct FarmEmployeeMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
