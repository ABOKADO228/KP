#pragma once

#include <string>

namespace fasc::server::security {

class PasswordHasher {
public:
  std::string hash(std::string_view password) const;
  bool verify(std::string_view password, std::string_view encoded_hash) const;

  static constexpr int kIterations = 120000;
  static constexpr std::size_t kSaltSize = 16;
  static constexpr std::size_t kHashSize = 32;
};

} // namespace fasc::server::security
