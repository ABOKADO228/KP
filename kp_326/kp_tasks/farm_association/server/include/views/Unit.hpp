#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct UnitRowView {
  int id{};
  std::optional<std::string> code{};
  std::optional<std::string> name{};
};

struct UnitRowsView {
  std::vector<UnitRowView> rows{};
};

struct UnitMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
