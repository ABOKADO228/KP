#pragma once

#include <domain/Types.hpp>

#include <cstdint>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы contract.
struct ContractEntity {
  /// Значение колонки id.
  std::uint64_t id{};

  /// Значение колонки supplier_id.
  odb::nullable<std::uint64_t> supplierId;

  /// Значение колонки farm_id.
  odb::nullable<std::uint64_t> farmId;

  /// Значение колонки association_id.
  std::uint64_t associationId{};

  /// Значение колонки contract_number.
  odb::nullable<std::string> contractNumber;

  /// Значение колонки sign_date.
  fasc::server::domain::Date signDate;

  /// Значение колонки start_date.
  fasc::server::domain::Date startDate;

  /// Значение колонки end_date.
  odb::nullable<fasc::server::domain::Date> endDate;

  /// Значение колонки status.
  odb::nullable<fasc::server::domain::ContractStatus> status;

  /// Значение колонки description.
  odb::nullable<std::string> description;
};

#pragma db object(ContractEntity) table("contract")
#pragma db member(ContractEntity::id) id auto column("id")
#pragma db member(ContractEntity::supplierId) column("supplier_id")
#pragma db member(ContractEntity::farmId) column("farm_id")
#pragma db member(ContractEntity::associationId) column("association_id")
#pragma db member(ContractEntity::contractNumber) column("contract_number")
#pragma db member(ContractEntity::signDate) column("sign_date")
#pragma db member(ContractEntity::startDate) column("start_date")
#pragma db member(ContractEntity::endDate) column("end_date")
#pragma db member(ContractEntity::status) column("status")
#pragma db member(ContractEntity::description) column("description")

} // namespace fasc::server::persistence
