#pragma once

#include <domain/Types.hpp>

#include <odb/pgsql/details/endian-traits.hxx>
#include <odb/pgsql/traits.hxx>

#include <chrono>
#include <cstring>
#include <string>
#include <string_view>

namespace odb::pgsql {

template <>
struct default_value_traits<fasc::server::domain::Date, id_date> {
  using value_type = fasc::server::domain::Date;
  using query_type = fasc::server::domain::Date;
  using image_type = image_traits<id_date>::image_type;

  static void set_value(value_type& value, const image_type& image, bool isNull) {
    if (isNull) {
      value = value_type{};
      return;
    }

    const auto daysFromPostgresEpoch = details::endian_traits::ntoh(image);
    const std::chrono::sys_days postgresEpoch{
        std::chrono::year{2000} / std::chrono::month{1} / std::chrono::day{1}};
    value.value = std::chrono::year_month_day{
        postgresEpoch + std::chrono::days{daysFromPostgresEpoch}};
  }

  static void set_image(image_type& image, bool& isNull, const value_type& value) {
    const std::chrono::sys_days postgresEpoch{
        std::chrono::year{2000} / std::chrono::month{1} / std::chrono::day{1}};
    const std::chrono::sys_days date{value.value};
    const auto daysFromPostgresEpoch =
        static_cast<image_type>((date - postgresEpoch).count());

    isNull = false;
    image = details::endian_traits::hton(daysFromPostgresEpoch);
  }
};

template <typename Enum>
struct enum_string_value_traits {
  using value_type = Enum;
  using query_type = Enum;
  using image_type = image_traits<id_string>::image_type;

  static void set_value(value_type& value,
                        const details::buffer& buffer,
                        std::size_t size,
                        bool isNull) {
    if (isNull) {
      value = value_type{};
      return;
    }

    value = fasc::server::domain::parseEnum<value_type>(
        std::string_view{buffer.data(), size});
  }

  static void set_image(details::buffer& buffer,
                        std::size_t& size,
                        bool& isNull,
                        const value_type& value) {
    const std::string_view text = fasc::server::domain::enumToString(value);
    if (text.size() > buffer.capacity()) {
      buffer.capacity(text.size());
    }

    std::memcpy(buffer.data(), text.data(), text.size());
    size = text.size();
    isNull = false;
  }
};

#define FASC_DECLARE_ENUM_STRING_TRAITS(EnumType)                                      \
  template <>                                                                          \
  struct default_value_traits<fasc::server::domain::EnumType, id_string>               \
      : enum_string_value_traits<fasc::server::domain::EnumType> {};

FASC_DECLARE_ENUM_STRING_TRAITS(FarmStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(FarmType)
FASC_DECLARE_ENUM_STRING_TRAITS(FarmAssociationStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(FarmOwnerStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(FarmPlotStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(FarmPlotAssignmentStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(AssociationMemberStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(AssociationFarmsStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(ContractStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(PurchaseOrderStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(PurchaseRequisitionStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(SalesRequisitionStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(SupplierStatus)
FASC_DECLARE_ENUM_STRING_TRAITS(AssociationRoleCode)
FASC_DECLARE_ENUM_STRING_TRAITS(IdentityDocumentTypeCode)
FASC_DECLARE_ENUM_STRING_TRAITS(EmploymentStatusCode)
FASC_DECLARE_ENUM_STRING_TRAITS(EmployeePlotAssignmentType)
FASC_DECLARE_ENUM_STRING_TRAITS(UnitCode)
FASC_DECLARE_ENUM_STRING_TRAITS(CurrencyCode)

#undef FASC_DECLARE_ENUM_STRING_TRAITS

} // namespace odb::pgsql
