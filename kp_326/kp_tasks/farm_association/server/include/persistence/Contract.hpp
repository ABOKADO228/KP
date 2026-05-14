#pragma once

#include <odb/core.hxx>
#include <odb/nullable.hxx>

#include <string>

namespace fasc::server::persistence {

/// Сущность таблицы contract.
struct ContractEntity {
  /// Значение колонки id.
  int id{};

  /// Значение колонки supplier_id.
  odb::nullable<int> supplierId;

  /// Значение колонки farm_id.
  odb::nullable<int> farmId;

  /// Значение колонки association_id.
  int associationId{};

  /// Значение колонки contract_number.
  odb::nullable<std::string> contractNumber;

  /// Значение колонки sign_date.
  std::string signDate;

  /// Значение колонки start_date.
  std::string startDate;

  /// Значение колонки end_date.
  odb::nullable<std::string> endDate;

  /// Значение колонки status.
  odb::nullable<std::string> status;

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
