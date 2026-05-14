#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fasc::server::views {

struct FarmRowView {
  int id{};
  std::optional<std::string> name{};
  std::optional<std::string> legalName{};
  std::optional<std::string> legalAddress{};
  std::optional<std::string> actualAddress{};
  std::optional<std::string> inn{};
  std::optional<std::string> ogrn{};
  std::optional<std::string> status{};
  std::optional<std::string> farmType{};
};

struct FarmRowsView {
  std::vector<FarmRowView> rows{};
};

struct FarmMutationView {
  unsigned long long affectedRows{};
};

} // namespace fasc::server::views
