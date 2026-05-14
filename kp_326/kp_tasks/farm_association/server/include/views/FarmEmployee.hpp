#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmEmployeeRowView {
  std::uint64_t id{};
  std::uint64_t personId{};
  std::uint64_t farmId{};
  std::uint64_t roleId{};
  std::uint64_t employmentStatusId{};
  fasc::server::domain::Date hireDate{};
  std::optional<fasc::server::domain::Date> dismissalDate{};
  std::optional<double> salary{};
  std::optional<std::string> employmentContractNumber{};
  std::optional<bool> isPrimaryWorkplace{};
};

struct FarmEmployeeRowsView {
  std::vector<FarmEmployeeRowView> rows{};
};

struct FarmEmployeeMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
