#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PersonRowView {
  std::uint64_t id{};
  std::optional<std::string> firstName{};
  std::optional<std::string> lastName{};
  std::optional<std::string> middleName{};
  std::optional<fasc::server::domain::Date> birthDate{};
  std::optional<std::string> phone{};
  std::optional<std::string> email{};
  std::optional<std::string> address{};
};

struct PersonRowsView {
  std::vector<PersonRowView> rows{};
};

struct PersonMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
