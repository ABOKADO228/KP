#include <controllers/dto/User.hpp>
#include <marshalling/FarmRole.hpp>
#include <marshalling/User.hpp>
#include <persistence/FarmRole.hpp>
#include <utils/JsonAssertions.hpp>
#include <views/FarmRole.hpp>
#include <views/User.hpp>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <string>

namespace {

using fasc::server::tests::utils::expectJsonStringField;
using fasc::server::tests::utils::expectJsonUnsignedLongField;
using fasc::server::controllers::dto::CreateUserCommand;
using fasc::server::views::UserView;

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

TEST(UserMarshallingTests, MapsFarmRoleEntityToRowView) {
  fasc::server::persistence::FarmRoleEntity entity{};
  entity.id = 7;
  entity.name = std::string{"Manager"};

  const auto view = fasc::server::views::toView(entity);

  EXPECT_EQ(view.id, 7);
  ASSERT_TRUE(view.name.has_value());
  EXPECT_EQ(*view.name, "Manager");
  EXPECT_FALSE(view.description.has_value());
}

TEST(UserMarshallingTests, WritesFarmRoleRowViewToJson) {
  fasc::server::persistence::FarmRoleEntity entity{};
  entity.id = 7;
  entity.name = std::string{"Manager"};

  const auto view = fasc::server::views::toView(entity);
  const auto json = nlohmann::json::parse(nlohmann::json(view).dump());

  EXPECT_EQ(json.at("resource").get<std::string>(), "farm_role");
  EXPECT_EQ(json.at("data").at("id").get<int>(), 7);
  EXPECT_EQ(json.at("data").at("name").get<std::string>(), "Manager");
  EXPECT_TRUE(json.at("data").at("description").is_null());
}

TEST(UserMarshallingTests, WritesFarmRoleRowsViewToJson) {
  fasc::server::persistence::FarmRoleEntity entity{};
  entity.id = 7;
  entity.name = std::string{"Manager"};

  const auto view = fasc::server::views::toView(std::vector{entity});
  const auto json = nlohmann::json::parse(nlohmann::json(view).dump());

  EXPECT_EQ(json.at("resource").get<std::string>(), "farm_role");
  ASSERT_TRUE(json.at("rows").is_array());
  ASSERT_EQ(json.at("rows").size(), 1U);
  EXPECT_EQ(json.at("rows").at(0).at("id").get<int>(), 7);
  EXPECT_EQ(json.at("rows").at(0).at("name").get<std::string>(), "Manager");
  EXPECT_TRUE(json.at("rows").at(0).at("description").is_null());
}

} // namespace
