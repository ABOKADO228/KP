#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct SupplierRowView {
  std::uint64_t id{};
  std::optional<std::string> name{};
  std::optional<std::string> legalAddress{};
  std::optional<fasc::server::domain::SupplierStatus> status{};
};

struct SupplierRowsView {
  std::vector<SupplierRowView> rows{};
};

struct SupplierMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
