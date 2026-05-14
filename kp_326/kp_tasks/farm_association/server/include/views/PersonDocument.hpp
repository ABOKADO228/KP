#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PersonDocumentRowView {
  std::uint64_t id{};
  std::uint64_t personId{};
  std::uint64_t documentTypeId{};
  std::optional<std::string> documentNumber{};
  std::optional<std::string> issuedBy{};
  std::optional<fasc::server::domain::Date> issuedDate{};
  std::optional<fasc::server::domain::Date> expirationDate{};
  std::optional<bool> isPrimary{};
};

struct PersonDocumentRowsView {
  std::vector<PersonDocumentRowView> rows{};
};

struct PersonDocumentMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
