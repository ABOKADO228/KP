#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct ProductRowView {
  int id{};
  int typeId{};
  std::optional<std::string> name{};
  std::optional<int> unitId{};
};

struct ProductRowsView {
  std::vector<ProductRowView> rows{};
};

struct ProductMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
