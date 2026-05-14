#include <security/JwtService.hpp>

#include <controllers/dto/User.hpp>

#include <gtest/gtest.h>

#include <string>

namespace {

using fasc::server::controllers::dto::UserDto;

TEST(JwtServiceTests, IssuesThreePartJwt) {
  const fasc::server::security::JwtService jwt{"test-secret"};

  const std::string token = jwt.issue(UserDto{42, "Alex"});

  const std::size_t first_dot = token.find('.');
  const std::size_t second_dot = token.find('.', first_dot + 1);

  EXPECT_NE(first_dot, std::string::npos);
  EXPECT_NE(second_dot, std::string::npos);
  EXPECT_EQ(token.find('.', second_dot + 1), std::string::npos);
}

TEST(JwtServiceTests, RejectsEmptySecret) {
  EXPECT_THROW((void)fasc::server::security::JwtService{""}, std::invalid_argument);
}

} // namespace
