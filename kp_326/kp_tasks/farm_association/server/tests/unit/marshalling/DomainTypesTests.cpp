#include <database/SqlValue.hpp>
#include <domain/Types.hpp>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <cstdint>
#include <limits>
#include <optional>
#include <string>

namespace {

namespace database = fasc::server::database;
namespace domain = fasc::server::domain;

template <typename Enum>
void expectEnumRoundTrip() {
  for (const auto& [value, text] : domain::EnumTraits<Enum>::values) {
    const std::string expected{text};

    EXPECT_EQ(std::string{domain::enumToString(value)}, expected);
    EXPECT_EQ(domain::parseEnum<Enum>(expected), value);
    EXPECT_EQ(domain::toSqlText(value), expected);
    EXPECT_EQ(domain::parseSqlText<Enum>(expected), value);

    const nlohmann::json json = value;
    EXPECT_EQ(json.get<std::string>(), expected);
    EXPECT_EQ(json.get<Enum>(), value);
  }
}

TEST(DomainTypesTests, RoundTripsAllDomainEnumsThroughJsonAndSqlText) {
  expectEnumRoundTrip<domain::FarmStatus>();
  expectEnumRoundTrip<domain::FarmType>();
  expectEnumRoundTrip<domain::FarmAssociationStatus>();
  expectEnumRoundTrip<domain::FarmOwnerStatus>();
  expectEnumRoundTrip<domain::FarmPlotStatus>();
  expectEnumRoundTrip<domain::FarmPlotAssignmentStatus>();
  expectEnumRoundTrip<domain::AssociationMemberStatus>();
  expectEnumRoundTrip<domain::AssociationFarmsStatus>();
  expectEnumRoundTrip<domain::ContractStatus>();
  expectEnumRoundTrip<domain::PurchaseOrderStatus>();
  expectEnumRoundTrip<domain::PurchaseRequisitionStatus>();
  expectEnumRoundTrip<domain::SalesRequisitionStatus>();
  expectEnumRoundTrip<domain::SupplierStatus>();
  expectEnumRoundTrip<domain::AssociationRoleCode>();
  expectEnumRoundTrip<domain::IdentityDocumentTypeCode>();
  expectEnumRoundTrip<domain::EmploymentStatusCode>();
  expectEnumRoundTrip<domain::EmployeePlotAssignmentType>();
  expectEnumRoundTrip<domain::UnitCode>();
  expectEnumRoundTrip<domain::CurrencyCode>();
}

TEST(DomainTypesTests, RoundTripsDateThroughJsonAndSqlText) {
  const auto date = domain::parseDate("2026-05-15");

  EXPECT_EQ(domain::formatDate(date), "2026-05-15");
  EXPECT_EQ(domain::toSqlText(date), "2026-05-15");
  EXPECT_EQ(domain::formatDate(domain::parseSqlText<domain::Date>("2026-05-15")),
            "2026-05-15");

  const nlohmann::json json = date;
  EXPECT_EQ(json.get<std::string>(), "2026-05-15");
  EXPECT_EQ(domain::formatDate(json.get<domain::Date>()), "2026-05-15");
}

TEST(DomainTypesTests, RejectsInvalidDateAndEnumText) {
  EXPECT_THROW(domain::parseDate("2026-02-30"), std::invalid_argument);
  EXPECT_THROW(domain::parseDate("15.05.2026"), std::invalid_argument);
  EXPECT_THROW(domain::parseEnum<domain::FarmStatus>("enabled"), std::invalid_argument);
}

TEST(DomainTypesTests, ConvertsSqlRowsToStrongTypes) {
  const database::SqlRow row{
      {"id", std::optional<std::string>{std::to_string(std::numeric_limits<std::uint64_t>::max())}},
      {"date", std::optional<std::string>{"2026-05-15"}},
      {"status", std::optional<std::string>{"active"}},
      {"enabled", std::optional<std::string>{"t"}},
      {"disabled", std::optional<std::string>{"0"}},
      {"missing_value", std::nullopt},
  };

  EXPECT_EQ(database::requireColumn<std::uint64_t>(row, "id"),
            std::numeric_limits<std::uint64_t>::max());
  EXPECT_EQ(domain::formatDate(database::requireColumn<domain::Date>(row, "date")),
            "2026-05-15");
  EXPECT_EQ(database::requireColumn<domain::FarmStatus>(row, "status"),
            domain::FarmStatus::Active);
  EXPECT_TRUE(database::requireColumn<bool>(row, "enabled"));
  EXPECT_FALSE(database::requireColumn<bool>(row, "disabled"));
  EXPECT_EQ(database::optionalColumn<std::uint64_t>(row, "missing_value"), std::nullopt);
}

TEST(DomainTypesTests, CreatesSqlParametersFromStrongTypes) {
  const auto id = database::makeSqlParameter(std::uint64_t{42});
  const auto date = database::makeSqlParameter(domain::parseDate("2026-05-15"));
  const auto currency = database::makeSqlParameter(domain::CurrencyCode::Rub);
  const auto enabled = database::makeSqlParameter(true);

  EXPECT_EQ(id.text, "42");
  EXPECT_FALSE(id.isNull);
  EXPECT_EQ(date.text, "2026-05-15");
  EXPECT_FALSE(date.isNull);
  EXPECT_EQ(currency.text, "RUB");
  EXPECT_FALSE(currency.isNull);
  EXPECT_EQ(enabled.text, "1");
  EXPECT_FALSE(enabled.isNull);
}

} // namespace
