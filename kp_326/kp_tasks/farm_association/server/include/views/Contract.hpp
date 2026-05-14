#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct ContractRowView {
  std::uint64_t id{};
  std::optional<std::uint64_t> supplierId{};
  std::optional<std::uint64_t> farmId{};
  std::uint64_t associationId{};
  std::optional<std::string> contractNumber{};
  fasc::server::domain::Date signDate{};
  fasc::server::domain::Date startDate{};
  std::optional<fasc::server::domain::Date> endDate{};
  std::optional<fasc::server::domain::ContractStatus> status{};
  std::optional<std::string> description{};
};

struct ContractRowsView {
  std::vector<ContractRowView> rows{};
};

struct ContractMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
