#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmAssociationRowView {
  std::uint64_t id{};
  std::optional<std::string> inn{};
  std::optional<std::string> ogrn{};
  std::optional<std::string> name{};
  std::optional<std::string> legalAddress{};
  std::optional<fasc::server::domain::FarmAssociationStatus> status{};
};

struct FarmAssociationRowsView {
  std::vector<FarmAssociationRowView> rows{};
};

struct FarmAssociationMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
