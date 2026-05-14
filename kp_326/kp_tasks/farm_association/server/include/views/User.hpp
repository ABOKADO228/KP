#pragma once

#include <cstdint>

#include <string>

namespace fasc::server::views {

struct UserView {
  std::uint64_t id{};
  std::string name{};
};

} // namespace fasc::server::views
