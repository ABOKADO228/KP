#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct EmploymentStatusRowView {
  int id{};
  std::optional<std::string> name{};
};

struct EmploymentStatusRowsView {
  std::vector<EmploymentStatusRowView> rows{};
};

struct EmploymentStatusMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
