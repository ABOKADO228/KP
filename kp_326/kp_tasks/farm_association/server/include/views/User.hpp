#pragma once

#include <string>

namespace fasc::server::views {

/// Public user representation returned through HTTP.
struct UserView {
  std::string login{};
  std::string role{};
};

} // namespace fasc::server::views
