#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct ProductTypeRowView {
  int id{};
  std::optional<int> parentId{};
  std::optional<std::string> sku{};
  int productLevel{};
  std::optional<std::string> name{};
  std::optional<std::string> description{};
};

struct ProductTypeRowsView {
  std::vector<ProductTypeRowView> rows{};
};

struct ProductTypeMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
