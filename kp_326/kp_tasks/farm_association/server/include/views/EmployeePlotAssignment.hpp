#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct EmployeePlotAssignmentRowView {
  std::uint64_t id{};
  std::uint64_t farmEmployeeId{};
  std::uint64_t farmPlotId{};
  std::optional<fasc::server::domain::EmployeePlotAssignmentType> assignmentType{};
  fasc::server::domain::Date assignedAt{};
  std::optional<fasc::server::domain::Date> unassignedAt{};
  std::optional<std::string> notes{};
};

struct EmployeePlotAssignmentRowsView {
  std::vector<EmployeePlotAssignmentRowView> rows{};
};

struct EmployeePlotAssignmentMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
