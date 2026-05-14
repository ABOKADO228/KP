#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct SupplierRowView {
  int id{};
  std::optional<std::string> name{};
  std::optional<std::string> legalAddress{};
  std::optional<std::string> status{};
};

struct SupplierRowsView {
  std::vector<SupplierRowView> rows{};
};

struct SupplierMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
