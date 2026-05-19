#include <domain/Types.hpp>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <string>

namespace {

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

} // namespace
