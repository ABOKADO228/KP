#include <database/Database.hpp>

#include <persistence/association-employee-odb.hxx>
#include <persistence/association-farms-odb.hxx>
#include <persistence/association-member-odb.hxx>
#include <persistence/association-role-odb.hxx>
#include <persistence/contract-odb.hxx>
#include <persistence/employee-plot-assignment-odb.hxx>
#include <persistence/employment-status-odb.hxx>
#include <persistence/farm-odb.hxx>
#include <persistence/farm-association-odb.hxx>
#include <persistence/farm-employee-odb.hxx>
#include <persistence/farm-owner-odb.hxx>
#include <persistence/farm-ownership-odb.hxx>
#include <persistence/farm-plot-odb.hxx>
#include <persistence/farm-plot-assignment-odb.hxx>
#include <persistence/farm-plot-consumption-product-odb.hxx>
#include <persistence/farm-plot-production-product-odb.hxx>
#include <persistence/farm-plot-type-odb.hxx>
#include <persistence/farm-role-odb.hxx>
#include <persistence/identity-document-type-odb.hxx>
#include <persistence/person-odb.hxx>
#include <persistence/person-document-odb.hxx>
#include <persistence/product-odb.hxx>
#include <persistence/product-type-odb.hxx>
#include <persistence/purchase-order-odb.hxx>
#include <persistence/purchase-order-item-odb.hxx>
#include <persistence/purchase-requisition-odb.hxx>
#include <persistence/sales-requisition-odb.hxx>
#include <persistence/supplier-odb.hxx>
#include <persistence/supplier-product-price-odb.hxx>
#include <persistence/unit-odb.hxx>

namespace fasc::server::database {

#define FARM_DEFINE_ENTITY_ACCESS(EntityType) \
  template <> \
  std::vector<EntityType> EntityAccess<EntityType>::select(Database& db) { \
    return Database::materialize<EntityType>(db.underlying->query<EntityType>(false)); \
  } \
  template <> \
  void EntityAccess<EntityType>::persist(Database& db, EntityType& entity) { \
    db.underlying->persist(entity); \
  } \
  template <> \
  void EntityAccess<EntityType>::update(Database& db, EntityType& entity) { \
    db.underlying->update(entity); \
  } \
  template <> \
  void EntityAccess<EntityType>::erase(Database& db, EntityType& entity) { \
    db.underlying->erase(entity); \
  }

FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::AssociationEmployeeEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::AssociationFarmsEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::AssociationMemberEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::AssociationRoleEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::ContractEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::EmployeePlotAssignmentEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::EmploymentStatusEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmAssociationEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmEmployeeEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmOwnerEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmOwnershipEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmPlotEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmPlotAssignmentEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmPlotConsumptionProductEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmPlotProductionProductEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmPlotTypeEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::FarmRoleEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::IdentityDocumentTypeEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::PersonEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::PersonDocumentEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::ProductEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::ProductTypeEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::PurchaseOrderEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::PurchaseOrderItemEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::PurchaseRequisitionEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::SalesRequisitionEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::SupplierEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::SupplierProductPriceEntity)
FARM_DEFINE_ENTITY_ACCESS(fasc::server::persistence::UnitEntity)

#undef FARM_DEFINE_ENTITY_ACCESS

} // namespace fasc::server::database
