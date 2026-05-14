#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct ProductTypeRowView {
  std::uint64_t id{};
  std::optional<std::uint64_t> parentId{};
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
