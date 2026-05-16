#include <security/JwtService.hpp>

#include <controllers/dto/User.hpp>

#include <gtest/gtest.h>

#include <string>

namespace {

using fasc::server::controllers::dto::UserDto;

TEST(JwtServiceTests, IssuesThreePartJwt) {
  const fasc::server::security::JwtService jwt{"test-secret"};

  const std::string token = jwt.issue(UserDto{"alex", "farm_worker"});

  const std::size_t first_dot = token.find('.');
  const std::size_t second_dot = token.find('.', first_dot + 1);

  EXPECT_NE(first_dot, std::string::npos);
  EXPECT_NE(second_dot, std::string::npos);
  EXPECT_EQ(token.find('.', second_dot + 1), std::string::npos);
}

TEST(JwtServiceTests, VerifiesIssuedTokenClaims) {
  const fasc::server::security::JwtService jwt{"test-secret"};

  const std::string token = jwt.issue(UserDto{"alex", "farm_worker"});
  const auto verified = jwt.verify(token);

  ASSERT_TRUE(verified.has_value());
  EXPECT_EQ(verified->login, "alex");
  EXPECT_EQ(verified->role, "farm_worker");
}

TEST(JwtServiceTests, RejectsTokenSignedWithAnotherSecret) {
  const fasc::server::security::JwtService issuer{"test-secret"};
  const fasc::server::security::JwtService verifier{"another-secret"};

  const std::string token = issuer.issue(UserDto{"alex", "farm_worker"});

  EXPECT_FALSE(verifier.verify(token).has_value());
}

TEST(JwtServiceTests, RejectsEmptySecret) {
  EXPECT_THROW((void)fasc::server::security::JwtService{""}, std::invalid_argument);
}

} // namespace
