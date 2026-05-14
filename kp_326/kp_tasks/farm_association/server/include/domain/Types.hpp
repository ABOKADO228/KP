#pragma once

#include <array>
#include <chrono>
#include <charconv>
#include <cstdint>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <utility>

#include <nlohmann/json.hpp>

namespace fasc::server::domain {

struct Date {
  std::chrono::year_month_day value{std::chrono::year{1970} / std::chrono::month{1} /
                                    std::chrono::day{1}};
};

inline std::string formatDate(const Date& date) {
  const auto year = static_cast<int>(date.value.year());
  const auto month = static_cast<unsigned>(date.value.month());
  const auto day = static_cast<unsigned>(date.value.day());

  std::ostringstream stream;
  stream << std::setfill('0') << std::setw(4) << year << '-'
         << std::setw(2) << month << '-' << std::setw(2) << day;
  return stream.str();
}

inline int parseDateComponent(std::string_view value, std::string_view field) {
  int parsed{};
  const auto* begin = value.data();
  const auto* end = value.data() + value.size();
  const auto result = std::from_chars(begin, end, parsed);
  if (result.ec != std::errc{} || result.ptr != end) {
    throw std::invalid_argument{"Invalid " + std::string{field} + ": " + std::string{value}};
  }
  return parsed;
}

inline Date parseDate(std::string_view value) {
  if (value.size() != 10 || value[4] != '-' || value[7] != '-') {
    throw std::invalid_argument{"Invalid date: " + std::string{value}};
  }

  const int year = parseDateComponent(value.substr(0, 4), "date year");
  const int month = parseDateComponent(value.substr(5, 2), "date month");
  const int day = parseDateComponent(value.substr(8, 2), "date day");

  const std::chrono::year_month_day parsed{
      std::chrono::year{year}, std::chrono::month{static_cast<unsigned>(month)},
      std::chrono::day{static_cast<unsigned>(day)}};
  if (!parsed.ok()) {
    throw std::invalid_argument{"Invalid date: " + std::string{value}};
  }

  return Date{parsed};
}

enum class FarmStatus {
  Active,
  Inactive,
  Suspended,
  Archived,
};

enum class FarmType {
  PeasantFarm,
  Llc,
  IndividualEntrepreneur,
  PersonalSubsidiary,
};

enum class FarmAssociationStatus {
  Active,
  Inactive,
  Suspended,
};

enum class FarmOwnerStatus {
  Active,
  Inactive,
  Archived,
};

enum class FarmPlotStatus {
  Active,
  Archived,
  Suspended,
  Inactive,
};

enum class FarmPlotAssignmentStatus {
  Active,
  Inactive,
  Suspended,
};

enum class AssociationMemberStatus {
  Active,
  Inactive,
  Expelled,
  Deceased,
};

enum class AssociationFarmsStatus {
  Active,
  Inactive,
  Suspended,
};

enum class ContractStatus {
  Expired,
  Active,
  Terminated,
};

enum class PurchaseOrderStatus {
  Received,
  Draft,
  Sent,
  Archived,
  Cancelled,
};

enum class PurchaseRequisitionStatus {
  Completed,
  Approved,
  Draft,
  Cancelled,
  Delivering,
};

enum class SalesRequisitionStatus {
  Approved,
  Delivering,
  Draft,
  Completed,
  Cancelled,
};

enum class SupplierStatus {
  Active,
  Inactive,
  Suspended,
  Archived,
};

enum class AssociationRoleCode {
  Chairman,
  ViceChairman,
  BoardMember,
  Accountant,
  Agronomist,
  ProcurementMgr,
  LogisticsMgr,
  Inspector,
};

enum class IdentityDocumentTypeCode {
  PassportRf,
  ForeignPassport,
  MilitaryId,
  ResidencePermit,
  BirthCertificate,
};

enum class EmploymentStatusCode {
  Active,
  Vacation,
  SickLeave,
  Probation,
  Terminated,
};

enum class EmployeePlotAssignmentType {
  Primary,
  Secondary,
  Temporary,
};

enum class UnitCode {
  Kg,
  T,
  L,
  Pcs,
  Box,
  Bag,
  M,
  M2,
  Ha,
  Head,
  Doz,
  Pack,
};

enum class CurrencyCode {
  Rub,
};

template <typename Enum>
struct EnumTraits;

template <typename Enum>
inline std::string_view enumToString(Enum value) {
  for (const auto& [enumValue, text] : EnumTraits<Enum>::values) {
    if (enumValue == value) {
      return text;
    }
  }

  throw std::invalid_argument{"Unsupported enum value"};
}

template <typename Enum>
inline Enum parseEnum(std::string_view value) {
  for (const auto& [enumValue, text] : EnumTraits<Enum>::values) {
    if (text == value) {
      return enumValue;
    }
  }

  throw std::invalid_argument{"Unknown enum value: " + std::string{value}};
}

template <typename T>
inline std::string toSqlText(const T& value) {
  using Value = std::remove_cvref_t<T>;
  if constexpr (std::is_same_v<Value, std::string>) {
    return value;
  } else if constexpr (std::is_same_v<Value, std::string_view>) {
    return std::string{value};
  } else if constexpr (std::is_same_v<Value, const char*>) {
    return std::string{value};
  } else if constexpr (std::is_same_v<Value, bool>) {
    return value ? "1" : "0";
  } else if constexpr (std::is_same_v<Value, Date>) {
    return formatDate(value);
  } else if constexpr (std::is_enum_v<Value>) {
    return std::string{enumToString(value)};
  } else if constexpr (std::is_integral_v<Value> || std::is_floating_point_v<Value>) {
    return std::to_string(value);
  } else {
    static_assert(std::is_same_v<Value, void>, "Unsupported SQL value type");
  }
}

template <typename T>
inline T parseSqlText(std::string_view value) {
  using Value = std::remove_cvref_t<T>;
  if constexpr (std::is_same_v<Value, std::string>) {
    return std::string{value};
  } else if constexpr (std::is_same_v<Value, bool>) {
    if (value == "1" || value == "true" || value == "t") {
      return true;
    }
    if (value == "0" || value == "false" || value == "f") {
      return false;
    }
    throw std::invalid_argument{"Invalid boolean value: " + std::string{value}};
  } else if constexpr (std::is_same_v<Value, Date>) {
    return parseDate(value);
  } else if constexpr (std::is_enum_v<Value>) {
    return parseEnum<Value>(value);
  } else if constexpr (std::is_integral_v<Value>) {
    Value parsed{};
    const auto* begin = value.data();
    const auto* end = value.data() + value.size();
    const auto result = std::from_chars(begin, end, parsed);
    if (result.ec != std::errc{} || result.ptr != end) {
      throw std::invalid_argument{"Invalid integer value: " + std::string{value}};
    }
    return parsed;
  } else if constexpr (std::is_floating_point_v<Value>) {
    std::size_t consumed{};
    const double parsed = std::stod(std::string{value}, &consumed);
    if (consumed != value.size()) {
      throw std::invalid_argument{"Invalid floating-point value: " + std::string{value}};
    }
    return static_cast<Value>(parsed);
  } else {
    static_assert(std::is_same_v<Value, void>, "Unsupported SQL value type");
  }
}

template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
inline void to_json(nlohmann::json& json, const Enum& value) {
  json = std::string{enumToString(value)};
}

template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
inline void from_json(const nlohmann::json& json, Enum& value) {
  value = parseEnum<Enum>(json.get<std::string>());
}

inline void to_json(nlohmann::json& json, const Date& value) {
  json = formatDate(value);
}

inline void from_json(const nlohmann::json& json, Date& value) {
  value = parseDate(json.get<std::string>());
}

template <typename T>
inline bool alwaysFalse = false;

template <>
struct EnumTraits<FarmStatus> {
  static constexpr std::array<std::pair<FarmStatus, std::string_view>, 4> values{{
      {FarmStatus::Active, "active"},
      {FarmStatus::Inactive, "inactive"},
      {FarmStatus::Suspended, "suspended"},
      {FarmStatus::Archived, "archived"},
  }};
};

template <>
struct EnumTraits<FarmType> {
  static constexpr std::array<std::pair<FarmType, std::string_view>, 4> values{{
      {FarmType::PeasantFarm, "peasant_farm"},
      {FarmType::Llc, "llc"},
      {FarmType::IndividualEntrepreneur, "individual_entrepreneur"},
      {FarmType::PersonalSubsidiary, "personal_subsidiary"},
  }};
};

template <>
struct EnumTraits<FarmAssociationStatus> {
  static constexpr std::array<std::pair<FarmAssociationStatus, std::string_view>, 3> values{{
      {FarmAssociationStatus::Active, "active"},
      {FarmAssociationStatus::Inactive, "inactive"},
      {FarmAssociationStatus::Suspended, "suspended"},
  }};
};

template <>
struct EnumTraits<FarmOwnerStatus> {
  static constexpr std::array<std::pair<FarmOwnerStatus, std::string_view>, 3> values{{
      {FarmOwnerStatus::Active, "active"},
      {FarmOwnerStatus::Inactive, "inactive"},
      {FarmOwnerStatus::Archived, "archived"},
  }};
};

template <>
struct EnumTraits<FarmPlotStatus> {
  static constexpr std::array<std::pair<FarmPlotStatus, std::string_view>, 4> values{{
      {FarmPlotStatus::Active, "active"},
      {FarmPlotStatus::Archived, "archived"},
      {FarmPlotStatus::Suspended, "suspended"},
      {FarmPlotStatus::Inactive, "inactive"},
  }};
};

template <>
struct EnumTraits<FarmPlotAssignmentStatus> {
  static constexpr std::array<std::pair<FarmPlotAssignmentStatus, std::string_view>, 3> values{{
      {FarmPlotAssignmentStatus::Active, "active"},
      {FarmPlotAssignmentStatus::Inactive, "inactive"},
      {FarmPlotAssignmentStatus::Suspended, "suspended"},
  }};
};

template <>
struct EnumTraits<AssociationMemberStatus> {
  static constexpr std::array<std::pair<AssociationMemberStatus, std::string_view>, 4> values{{
      {AssociationMemberStatus::Active, "active"},
      {AssociationMemberStatus::Inactive, "inactive"},
      {AssociationMemberStatus::Expelled, "expelled"},
      {AssociationMemberStatus::Deceased, "deceased"},
  }};
};

template <>
struct EnumTraits<AssociationFarmsStatus> {
  static constexpr std::array<std::pair<AssociationFarmsStatus, std::string_view>, 3> values{{
      {AssociationFarmsStatus::Active, "active"},
      {AssociationFarmsStatus::Inactive, "inactive"},
      {AssociationFarmsStatus::Suspended, "suspended"},
  }};
};

template <>
struct EnumTraits<ContractStatus> {
  static constexpr std::array<std::pair<ContractStatus, std::string_view>, 3> values{{
      {ContractStatus::Expired, "expired"},
      {ContractStatus::Active, "active"},
      {ContractStatus::Terminated, "terminated"},
  }};
};

template <>
struct EnumTraits<PurchaseOrderStatus> {
  static constexpr std::array<std::pair<PurchaseOrderStatus, std::string_view>, 5> values{{
      {PurchaseOrderStatus::Received, "received"},
      {PurchaseOrderStatus::Draft, "draft"},
      {PurchaseOrderStatus::Sent, "sent"},
      {PurchaseOrderStatus::Archived, "archived"},
      {PurchaseOrderStatus::Cancelled, "cancelled"},
  }};
};

template <>
struct EnumTraits<PurchaseRequisitionStatus> {
  static constexpr std::array<std::pair<PurchaseRequisitionStatus, std::string_view>, 5> values{{
      {PurchaseRequisitionStatus::Completed, "completed"},
      {PurchaseRequisitionStatus::Approved, "approved"},
      {PurchaseRequisitionStatus::Draft, "draft"},
      {PurchaseRequisitionStatus::Cancelled, "cancelled"},
      {PurchaseRequisitionStatus::Delivering, "delivering"},
  }};
};

template <>
struct EnumTraits<SalesRequisitionStatus> {
  static constexpr std::array<std::pair<SalesRequisitionStatus, std::string_view>, 5> values{{
      {SalesRequisitionStatus::Approved, "approved"},
      {SalesRequisitionStatus::Delivering, "delivering"},
      {SalesRequisitionStatus::Draft, "draft"},
      {SalesRequisitionStatus::Completed, "completed"},
      {SalesRequisitionStatus::Cancelled, "cancelled"},
  }};
};

template <>
struct EnumTraits<SupplierStatus> {
  static constexpr std::array<std::pair<SupplierStatus, std::string_view>, 4> values{{
      {SupplierStatus::Active, "active"},
      {SupplierStatus::Inactive, "inactive"},
      {SupplierStatus::Suspended, "suspended"},
      {SupplierStatus::Archived, "archived"},
  }};
};

template <>
struct EnumTraits<AssociationRoleCode> {
  static constexpr std::array<std::pair<AssociationRoleCode, std::string_view>, 8> values{{
      {AssociationRoleCode::Chairman, "chairman"},
      {AssociationRoleCode::ViceChairman, "vice_chairman"},
      {AssociationRoleCode::BoardMember, "board_member"},
      {AssociationRoleCode::Accountant, "accountant"},
      {AssociationRoleCode::Agronomist, "agronomist"},
      {AssociationRoleCode::ProcurementMgr, "procurement_mgr"},
      {AssociationRoleCode::LogisticsMgr, "logistics_mgr"},
      {AssociationRoleCode::Inspector, "inspector"},
  }};
};

template <>
struct EnumTraits<IdentityDocumentTypeCode> {
  static constexpr std::array<std::pair<IdentityDocumentTypeCode, std::string_view>, 5> values{{
      {IdentityDocumentTypeCode::PassportRf, "passport_rf"},
      {IdentityDocumentTypeCode::ForeignPassport, "foreign_passport"},
      {IdentityDocumentTypeCode::MilitaryId, "military_id"},
      {IdentityDocumentTypeCode::ResidencePermit, "residence_permit"},
      {IdentityDocumentTypeCode::BirthCertificate, "birth_certificate"},
  }};
};

template <>
struct EnumTraits<EmploymentStatusCode> {
  static constexpr std::array<std::pair<EmploymentStatusCode, std::string_view>, 5> values{{
      {EmploymentStatusCode::Active, "active"},
      {EmploymentStatusCode::Vacation, "vacation"},
      {EmploymentStatusCode::SickLeave, "sick_leave"},
      {EmploymentStatusCode::Probation, "probation"},
      {EmploymentStatusCode::Terminated, "terminated"},
  }};
};

template <>
struct EnumTraits<EmployeePlotAssignmentType> {
  static constexpr std::array<std::pair<EmployeePlotAssignmentType, std::string_view>, 3> values{{
      {EmployeePlotAssignmentType::Primary, "primary"},
      {EmployeePlotAssignmentType::Secondary, "secondary"},
      {EmployeePlotAssignmentType::Temporary, "temporary"},
  }};
};

template <>
struct EnumTraits<UnitCode> {
  static constexpr std::array<std::pair<UnitCode, std::string_view>, 12> values{{
      {UnitCode::Kg, "kg"},
      {UnitCode::T, "t"},
      {UnitCode::L, "l"},
      {UnitCode::Pcs, "pcs"},
      {UnitCode::Box, "box"},
      {UnitCode::Bag, "bag"},
      {UnitCode::M, "m"},
      {UnitCode::M2, "m2"},
      {UnitCode::Ha, "ha"},
      {UnitCode::Head, "head"},
      {UnitCode::Doz, "doz"},
      {UnitCode::Pack, "pack"},
  }};
};

template <>
struct EnumTraits<CurrencyCode> {
  static constexpr std::array<std::pair<CurrencyCode, std::string_view>, 1> values{{
      {CurrencyCode::Rub, "RUB"},
  }};
};

} // namespace fasc::server::domain
