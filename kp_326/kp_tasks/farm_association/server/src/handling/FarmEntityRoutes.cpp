#include <handling/FarmEntityRoutes.hpp>

#include <handling/AssociationEmployee.hpp>
#include <handling/AssociationFarms.hpp>
#include <handling/AssociationMember.hpp>
#include <handling/AssociationRole.hpp>
#include <handling/Contract.hpp>
#include <handling/EmployeePlotAssignment.hpp>
#include <handling/EmploymentStatus.hpp>
#include <handling/Farm.hpp>
#include <handling/FarmAssociation.hpp>
#include <handling/FarmEmployee.hpp>
#include <handling/FarmOwner.hpp>
#include <handling/FarmOwnership.hpp>
#include <handling/FarmPlot.hpp>
#include <handling/FarmPlotAssignment.hpp>
#include <handling/FarmPlotConsumptionProduct.hpp>
#include <handling/FarmPlotProductionProduct.hpp>
#include <handling/FarmPlotType.hpp>
#include <handling/FarmRole.hpp>
#include <handling/IdentityDocumentType.hpp>
#include <handling/Person.hpp>
#include <handling/PersonDocument.hpp>
#include <handling/Product.hpp>
#include <handling/ProductType.hpp>
#include <handling/PurchaseOrder.hpp>
#include <handling/PurchaseOrderItem.hpp>
#include <handling/PurchaseRequisition.hpp>
#include <handling/SalesRequisition.hpp>
#include <handling/Supplier.hpp>
#include <handling/SupplierProductPrice.hpp>
#include <handling/Unit.hpp>

namespace fasc::server::handling {

void registerFarmEntityRoutes(fasc::server::core::Server& server,
                              fasc::server::database::Database& database) {
  registerAssociationEmployeeRoutes(server, database);
  registerAssociationFarmsRoutes(server, database);
  registerAssociationMemberRoutes(server, database);
  registerAssociationRoleRoutes(server, database);
  registerContractRoutes(server, database);
  registerEmployeePlotAssignmentRoutes(server, database);
  registerEmploymentStatusRoutes(server, database);
  registerFarmRoutes(server, database);
  registerFarmAssociationRoutes(server, database);
  registerFarmEmployeeRoutes(server, database);
  registerFarmOwnerRoutes(server, database);
  registerFarmOwnershipRoutes(server, database);
  registerFarmPlotRoutes(server, database);
  registerFarmPlotAssignmentRoutes(server, database);
  registerFarmPlotConsumptionProductRoutes(server, database);
  registerFarmPlotProductionProductRoutes(server, database);
  registerFarmPlotTypeRoutes(server, database);
  registerFarmRoleRoutes(server, database);
  registerIdentityDocumentTypeRoutes(server, database);
  registerPersonRoutes(server, database);
  registerPersonDocumentRoutes(server, database);
  registerProductRoutes(server, database);
  registerProductTypeRoutes(server, database);
  registerPurchaseOrderRoutes(server, database);
  registerPurchaseOrderItemRoutes(server, database);
  registerPurchaseRequisitionRoutes(server, database);
  registerSalesRequisitionRoutes(server, database);
  registerSupplierRoutes(server, database);
  registerSupplierProductPriceRoutes(server, database);
  registerUnitRoutes(server, database);
}

} // namespace fasc::server::handling
