#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct ProductRowView {
  std::uint64_t id{};
  std::uint64_t typeId{};
  std::optional<std::string> name{};
  std::optional<std::uint64_t> unitId{};
};

struct ProductRowsView {
  std::vector<ProductRowView> rows{};
};

struct ProductMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
