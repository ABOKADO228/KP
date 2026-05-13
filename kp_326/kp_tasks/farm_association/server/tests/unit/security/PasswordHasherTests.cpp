#include <security/PasswordHasher.hpp>

#include <gtest/gtest.h>

#include <string>

namespace {

TEST(PasswordHasherTests, HashesPasswordWithoutStoringPlainText) {
  const fasc::server::security::PasswordHasher hasher;

  const std::string hash = hasher.hash("password123");

  EXPECT_NE(hash.find("pbkdf2_sha256$"), std::string::npos);
  EXPECT_EQ(hash.find("password123"), std::string::npos);
}

TEST(PasswordHasherTests, VerifiesMatchingPassword) {
  const fasc::server::security::PasswordHasher hasher;

  const std::string hash = hasher.hash("password123");

  EXPECT_TRUE(hasher.verify("password123", hash));
  EXPECT_FALSE(hasher.verify("wrong-password", hash));
}

} // namespace
