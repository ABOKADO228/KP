#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct IdentityDocumentTypeRowView {
  std::uint64_t id{};
  std::optional<fasc::server::domain::IdentityDocumentTypeCode> code{};
  std::optional<std::string> name{};
  std::optional<std::string> description{};
};

struct IdentityDocumentTypeRowsView {
  std::vector<IdentityDocumentTypeRowView> rows{};
};

struct IdentityDocumentTypeMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
