#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct EmploymentStatusRowView {
  std::uint64_t id{};
  std::optional<fasc::server::domain::EmploymentStatusCode> name{};
};

struct EmploymentStatusRowsView {
  std::vector<EmploymentStatusRowView> rows{};
};

struct EmploymentStatusMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
