#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct IdentityDocumentTypeRowView {
  int id{};
  std::optional<std::string> code{};
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
