#pragma once

#include <string>
#include <vector>

namespace fasc::server::views {

/// Public user representation returned through HTTP.
struct UserView {
  std::string login{};
  std::string role{};
};

/// Public list of application users.
struct UserListView {
  std::vector<UserView> users;
};

} // namespace fasc::server::views
