#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct EmployeePlotAssignmentRowView {
  int id{};
  int farmEmployeeId{};
  int farmPlotId{};
  std::optional<std::string> assignmentType{};
  std::string assignedAt{};
  std::optional<std::string> unassignedAt{};
  std::optional<std::string> notes{};
};

struct EmployeePlotAssignmentRowsView {
  std::vector<EmployeePlotAssignmentRowView> rows{};
};

struct EmployeePlotAssignmentMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
