#pragma once

#include <chrono>
#include <string>

struct UserDto;

namespace fasc::server::security {

class JwtService {
public:
  JwtService(std::string secret,
             std::string issuer = "farm-association-server",
             std::chrono::seconds ttl = std::chrono::hours{24});

  std::string issue(const UserDto& user) const;

private:
  std::string secret_;
  std::string issuer_;
  std::chrono::seconds ttl_;
};

} // namespace fasc::server::security
