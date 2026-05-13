#include <controllers/dto/User.hpp>
#include <marshalling/User.hpp>
#include <utils/JsonAssertions.hpp>
#include <views/User.hpp>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <string>

namespace {

using fasc::server::tests::utils::expectJsonStringField;
using fasc::server::tests::utils::expectJsonUnsignedLongField;

TEST(UserMarshallingTests, ReadsCreateUserCommandFromJson) {
  const auto json = nlohmann::json{{"name", "Alex"}, {"password", "password123"}};

  const CreateUserCommand command = json.get<CreateUserCommand>();

  EXPECT_EQ(command.name, "Alex");
  EXPECT_EQ(command.password, "password123");
}

TEST(UserMarshallingTests, ThrowsWhenRequiredNameFieldIsMissing) {
  const auto json = nlohmann::json::object();

  EXPECT_THROW((void)json.get<CreateUserCommand>(), nlohmann::json::exception);
}

TEST(UserMarshallingTests, WritesUserViewToJson) {
  const UserView view{42, "Alex"};

  const std::string body = nlohmann::json(view).dump();

  expectJsonUnsignedLongField(body, "id", 42);
  expectJsonStringField(body, "name", "Alex");
}

} // namespace
