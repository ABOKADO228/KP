#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct PersonRowView {
  int id{};
  std::optional<std::string> firstName{};
  std::optional<std::string> lastName{};
  std::optional<std::string> middleName{};
  std::optional<std::string> birthDate{};
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
