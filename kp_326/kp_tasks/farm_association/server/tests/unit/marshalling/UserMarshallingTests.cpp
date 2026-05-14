#include <controllers/dto/User.hpp>
#include <domain/Types.hpp>
#include <marshalling/Farm.hpp>
#include <marshalling/FarmRole.hpp>
#include <marshalling/PersonDocument.hpp>
#include <marshalling/PurchaseOrder.hpp>
#include <marshalling/User.hpp>
#include <persistence/Farm.hpp>
#include <persistence/FarmRole.hpp>
#include <utils/JsonAssertions.hpp>
#include <views/Farm.hpp>
#include <views/FarmRole.hpp>
#include <views/PurchaseOrder.hpp>
#include <views/User.hpp>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <cstdint>
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
  EXPECT_EQ(json.at("data").at("id").get<std::uint64_t>(), 7);
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
  EXPECT_EQ(json.at("rows").at(0).at("id").get<std::uint64_t>(), 7);
  EXPECT_EQ(json.at("rows").at(0).at("name").get<std::string>(), "Manager");
  EXPECT_TRUE(json.at("rows").at(0).at("description").is_null());
}

TEST(UserMarshallingTests, ReadsFarmCreateDtoEnumFieldsFromJson) {
  const auto json =
      nlohmann::json{{"status", "active"}, {"farm_type", "peasant_farm"}};

  const auto dto = json.get<fasc::server::controllers::dto::FarmCreateDto>();

  ASSERT_TRUE(dto.status.has_value());
  ASSERT_TRUE(dto.farmType.has_value());
  EXPECT_EQ(*dto.status, fasc::server::domain::FarmStatus::Active);
  EXPECT_EQ(*dto.farmType, fasc::server::domain::FarmType::PeasantFarm);
}

TEST(UserMarshallingTests, ReadsPurchaseOrderDtoDateEnumAndUint64FromJson) {
  const auto json = nlohmann::json{
      {"association_id", 9007199254740991ULL},
      {"supplier_id", 42ULL},
      {"order_date", "2025-04-12"},
      {"expected_delivery_date", "2025-04-20"},
      {"status", "draft"},
      {"received_at", "2025-04-21"},
      {"created_by", 77ULL},
  };

  const auto dto = json.get<fasc::server::controllers::dto::PurchaseOrderCreateDto>();

  ASSERT_TRUE(dto.associationId.has_value());
  ASSERT_TRUE(dto.orderDate.has_value());
  ASSERT_TRUE(dto.expectedDeliveryDate.has_value());
  ASSERT_TRUE(dto.status.has_value());
  ASSERT_TRUE(dto.receivedAt.has_value());
  ASSERT_TRUE(dto.createdBy.has_value());
  EXPECT_EQ(*dto.associationId, 9007199254740991ULL);
  EXPECT_EQ(fasc::server::domain::formatDate(*dto.orderDate), "2025-04-12");
  EXPECT_EQ(fasc::server::domain::formatDate(*dto.expectedDeliveryDate), "2025-04-20");
  EXPECT_EQ(*dto.status, fasc::server::domain::PurchaseOrderStatus::Draft);
  EXPECT_EQ(fasc::server::domain::formatDate(*dto.receivedAt), "2025-04-21");
  EXPECT_EQ(*dto.createdBy, 77ULL);
}

TEST(UserMarshallingTests, WritesPurchaseOrderViewDateAndEnumToJson) {
  fasc::server::views::PurchaseOrderRowView view{};
  view.id = 99;
  view.associationId = 10;
  view.supplierId = 20;
  view.orderDate = fasc::server::domain::parseDate("2025-05-01");
  view.expectedDeliveryDate = fasc::server::domain::parseDate("2025-05-08");
  view.status = fasc::server::domain::PurchaseOrderStatus::Sent;
  view.receivedAt = fasc::server::domain::parseDate("2025-05-10");
  view.createdBy = 123;

  const auto json = nlohmann::json::parse(nlohmann::json(view).dump());

  EXPECT_EQ(json.at("resource").get<std::string>(), "purchase_order");
  EXPECT_EQ(json.at("data").at("id").get<std::uint64_t>(), 99);
  EXPECT_EQ(json.at("data").at("order_date").get<std::string>(), "2025-05-01");
  EXPECT_EQ(json.at("data").at("expected_delivery_date").get<std::string>(),
            "2025-05-08");
  EXPECT_EQ(json.at("data").at("status").get<std::string>(), "sent");
  EXPECT_EQ(json.at("data").at("received_at").get<std::string>(), "2025-05-10");
  EXPECT_EQ(json.at("data").at("created_by").get<std::uint64_t>(), 123);
}

TEST(UserMarshallingTests, ReadsPersonDocumentBoolFromJson) {
  const auto json = nlohmann::json{{"person_id", 1ULL},
                                   {"document_type_id", 2ULL},
                                   {"issued_date", "2025-01-01"},
                                   {"expiration_date", "2035-01-01"},
                                   {"is_primary", true}};

  const auto dto = json.get<fasc::server::controllers::dto::PersonDocumentCreateDto>();

  ASSERT_TRUE(dto.isPrimary.has_value());
  ASSERT_TRUE(dto.issuedDate.has_value());
  EXPECT_TRUE(*dto.isPrimary);
  EXPECT_EQ(fasc::server::domain::formatDate(*dto.issuedDate), "2025-01-01");
}

} // namespace
