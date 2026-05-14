#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct UnitRowView {
  std::uint64_t id{};
  std::optional<fasc::server::domain::UnitCode> code{};
  std::optional<std::string> name{};
};

struct UnitRowsView {
  std::vector<UnitRowView> rows{};
};

struct UnitMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
