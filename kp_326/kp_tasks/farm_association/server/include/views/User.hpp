#pragma once

#include <string>

namespace fasc::server::views {

struct UserView {
  unsigned long id{};
  std::string name{};
};

} // namespace fasc::server::views
