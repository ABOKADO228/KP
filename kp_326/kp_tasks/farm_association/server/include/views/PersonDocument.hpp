#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PersonDocumentRowView {
  int id{};
  int personId{};
  int documentTypeId{};
  std::optional<std::string> documentNumber{};
  std::optional<std::string> issuedBy{};
  std::optional<std::string> issuedDate{};
  std::optional<std::string> expirationDate{};
  std::optional<int> isPrimary{};
};

struct PersonDocumentRowsView {
  std::vector<PersonDocumentRowView> rows{};
};

struct PersonDocumentMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
