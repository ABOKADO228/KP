#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct ContractRowView {
  int id{};
  std::optional<int> supplierId{};
  std::optional<int> farmId{};
  int associationId{};
  std::optional<std::string> contractNumber{};
  std::string signDate{};
  std::string startDate{};
  std::optional<std::string> endDate{};
  std::optional<std::string> status{};
  std::optional<std::string> description{};
};

struct ContractRowsView {
  std::vector<ContractRowView> rows{};
};

struct ContractMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
