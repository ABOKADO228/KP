#include <handling/FarmEntityRoutes.hpp>

#include <controllers/app/AssociationEmployee.hpp>
#include <controllers/app/AssociationFarms.hpp>
#include <controllers/app/AssociationMember.hpp>
#include <controllers/app/AssociationRole.hpp>
#include <controllers/app/Contract.hpp>
#include <controllers/app/EmployeePlotAssignment.hpp>
#include <controllers/app/EmploymentStatus.hpp>
#include <controllers/app/Farm.hpp>
#include <controllers/app/FarmAssociation.hpp>
#include <controllers/app/FarmEmployee.hpp>
#include <controllers/app/FarmOwner.hpp>
#include <controllers/app/FarmOwnership.hpp>
#include <controllers/app/FarmPlot.hpp>
#include <controllers/app/FarmPlotAssignment.hpp>
#include <controllers/app/FarmPlotConsumptionProduct.hpp>
#include <controllers/app/FarmPlotProductionProduct.hpp>
#include <controllers/app/FarmPlotType.hpp>
#include <controllers/app/FarmRole.hpp>
#include <controllers/app/IdentityDocumentType.hpp>
#include <controllers/app/Person.hpp>
#include <controllers/app/PersonDocument.hpp>
#include <controllers/app/Product.hpp>
#include <controllers/app/ProductType.hpp>
#include <controllers/app/PurchaseOrder.hpp>
#include <controllers/app/PurchaseOrderItem.hpp>
#include <controllers/app/PurchaseRequisition.hpp>
#include <controllers/app/SalesRequisition.hpp>
#include <controllers/app/Supplier.hpp>
#include <controllers/app/SupplierProductPrice.hpp>
#include <controllers/app/Unit.hpp>
#include <controllers/http/AssociationEmployee.hpp>
#include <controllers/http/AssociationFarms.hpp>
#include <controllers/http/AssociationMember.hpp>
#include <controllers/http/AssociationRole.hpp>
#include <controllers/http/Contract.hpp>
#include <controllers/http/EmployeePlotAssignment.hpp>
#include <controllers/http/EmploymentStatus.hpp>
#include <controllers/http/Farm.hpp>
#include <controllers/http/FarmAssociation.hpp>
#include <controllers/http/FarmEmployee.hpp>
#include <controllers/http/FarmOwner.hpp>
#include <controllers/http/FarmOwnership.hpp>
#include <controllers/http/FarmPlot.hpp>
#include <controllers/http/FarmPlotAssignment.hpp>
#include <controllers/http/FarmPlotConsumptionProduct.hpp>
#include <controllers/http/FarmPlotProductionProduct.hpp>
#include <controllers/http/FarmPlotType.hpp>
#include <controllers/http/FarmRole.hpp>
#include <controllers/http/IdentityDocumentType.hpp>
#include <controllers/http/Person.hpp>
#include <controllers/http/PersonDocument.hpp>
#include <controllers/http/Product.hpp>
#include <controllers/http/ProductType.hpp>
#include <controllers/http/PurchaseOrder.hpp>
#include <controllers/http/PurchaseOrderItem.hpp>
#include <controllers/http/PurchaseRequisition.hpp>
#include <controllers/http/SalesRequisition.hpp>
#include <controllers/http/Supplier.hpp>
#include <controllers/http/SupplierProductPrice.hpp>
#include <controllers/http/Unit.hpp>
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

#include <memory>

namespace fasc::server::handling {

void registerFarmEntityRoutes(fasc::server::core::Server& server,
                              fasc::server::database::Database& database) {
  auto associationemployeeController = std::make_shared<fasc::server::controllers::app::AssociationEmployeeController>(database);
  auto associationemployeeHttpController = std::make_shared<fasc::server::controllers::http::AssociationEmployeeHttpController>(*associationemployeeController);
  auto associationemployeeHandler = std::make_shared<AssociationEmployeeHandler>(*associationemployeeHttpController);
  server.get("/api/association_employee", [associationemployeeController, associationemployeeHttpController, associationemployeeHandler](const fasc::server::core::HttpRequest& request) { return associationemployeeHandler->list(request); });
  server.post("/api/association_employee", [associationemployeeController, associationemployeeHttpController, associationemployeeHandler](const fasc::server::core::HttpRequest& request) { return associationemployeeHandler->create(request); });
  server.get("/api/association_employee/item", [associationemployeeController, associationemployeeHttpController, associationemployeeHandler](const fasc::server::core::HttpRequest& request) { return associationemployeeHandler->load(request); });
  server.put("/api/association_employee/item", [associationemployeeController, associationemployeeHttpController, associationemployeeHandler](const fasc::server::core::HttpRequest& request) { return associationemployeeHandler->update(request); });
  server.del("/api/association_employee/item", [associationemployeeController, associationemployeeHttpController, associationemployeeHandler](const fasc::server::core::HttpRequest& request) { return associationemployeeHandler->erase(request); });

  auto associationfarmsController = std::make_shared<fasc::server::controllers::app::AssociationFarmsController>(database);
  auto associationfarmsHttpController = std::make_shared<fasc::server::controllers::http::AssociationFarmsHttpController>(*associationfarmsController);
  auto associationfarmsHandler = std::make_shared<AssociationFarmsHandler>(*associationfarmsHttpController);
  server.get("/api/association_farms", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->list(request); });
  server.post("/api/association_farms", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->create(request); });
  server.get("/api/association_farms/item", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->load(request); });
  server.put("/api/association_farms/item", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->update(request); });
  server.del("/api/association_farms/item", [associationfarmsController, associationfarmsHttpController, associationfarmsHandler](const fasc::server::core::HttpRequest& request) { return associationfarmsHandler->erase(request); });

  auto associationmemberController = std::make_shared<fasc::server::controllers::app::AssociationMemberController>(database);
  auto associationmemberHttpController = std::make_shared<fasc::server::controllers::http::AssociationMemberHttpController>(*associationmemberController);
  auto associationmemberHandler = std::make_shared<AssociationMemberHandler>(*associationmemberHttpController);
  server.get("/api/association_member", [associationmemberController, associationmemberHttpController, associationmemberHandler](const fasc::server::core::HttpRequest& request) { return associationmemberHandler->list(request); });
  server.post("/api/association_member", [associationmemberController, associationmemberHttpController, associationmemberHandler](const fasc::server::core::HttpRequest& request) { return associationmemberHandler->create(request); });
  server.get("/api/association_member/item", [associationmemberController, associationmemberHttpController, associationmemberHandler](const fasc::server::core::HttpRequest& request) { return associationmemberHandler->load(request); });
  server.put("/api/association_member/item", [associationmemberController, associationmemberHttpController, associationmemberHandler](const fasc::server::core::HttpRequest& request) { return associationmemberHandler->update(request); });
  server.del("/api/association_member/item", [associationmemberController, associationmemberHttpController, associationmemberHandler](const fasc::server::core::HttpRequest& request) { return associationmemberHandler->erase(request); });

  auto associationroleController = std::make_shared<fasc::server::controllers::app::AssociationRoleController>(database);
  auto associationroleHttpController = std::make_shared<fasc::server::controllers::http::AssociationRoleHttpController>(*associationroleController);
  auto associationroleHandler = std::make_shared<AssociationRoleHandler>(*associationroleHttpController);
  server.get("/api/association_role", [associationroleController, associationroleHttpController, associationroleHandler](const fasc::server::core::HttpRequest& request) { return associationroleHandler->list(request); });
  server.post("/api/association_role", [associationroleController, associationroleHttpController, associationroleHandler](const fasc::server::core::HttpRequest& request) { return associationroleHandler->create(request); });
  server.get("/api/association_role/item", [associationroleController, associationroleHttpController, associationroleHandler](const fasc::server::core::HttpRequest& request) { return associationroleHandler->load(request); });
  server.put("/api/association_role/item", [associationroleController, associationroleHttpController, associationroleHandler](const fasc::server::core::HttpRequest& request) { return associationroleHandler->update(request); });
  server.del("/api/association_role/item", [associationroleController, associationroleHttpController, associationroleHandler](const fasc::server::core::HttpRequest& request) { return associationroleHandler->erase(request); });

  auto contractController = std::make_shared<fasc::server::controllers::app::ContractController>(database);
  auto contractHttpController = std::make_shared<fasc::server::controllers::http::ContractHttpController>(*contractController);
  auto contractHandler = std::make_shared<ContractHandler>(*contractHttpController);
  server.get("/api/contract", [contractController, contractHttpController, contractHandler](const fasc::server::core::HttpRequest& request) { return contractHandler->list(request); });
  server.post("/api/contract", [contractController, contractHttpController, contractHandler](const fasc::server::core::HttpRequest& request) { return contractHandler->create(request); });
  server.get("/api/contract/item", [contractController, contractHttpController, contractHandler](const fasc::server::core::HttpRequest& request) { return contractHandler->load(request); });
  server.put("/api/contract/item", [contractController, contractHttpController, contractHandler](const fasc::server::core::HttpRequest& request) { return contractHandler->update(request); });
  server.del("/api/contract/item", [contractController, contractHttpController, contractHandler](const fasc::server::core::HttpRequest& request) { return contractHandler->erase(request); });

  auto employeeplotassignmentController = std::make_shared<fasc::server::controllers::app::EmployeePlotAssignmentController>(database);
  auto employeeplotassignmentHttpController = std::make_shared<fasc::server::controllers::http::EmployeePlotAssignmentHttpController>(*employeeplotassignmentController);
  auto employeeplotassignmentHandler = std::make_shared<EmployeePlotAssignmentHandler>(*employeeplotassignmentHttpController);
  server.get("/api/employee_plot_assignment", [employeeplotassignmentController, employeeplotassignmentHttpController, employeeplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return employeeplotassignmentHandler->list(request); });
  server.post("/api/employee_plot_assignment", [employeeplotassignmentController, employeeplotassignmentHttpController, employeeplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return employeeplotassignmentHandler->create(request); });
  server.get("/api/employee_plot_assignment/item", [employeeplotassignmentController, employeeplotassignmentHttpController, employeeplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return employeeplotassignmentHandler->load(request); });
  server.put("/api/employee_plot_assignment/item", [employeeplotassignmentController, employeeplotassignmentHttpController, employeeplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return employeeplotassignmentHandler->update(request); });
  server.del("/api/employee_plot_assignment/item", [employeeplotassignmentController, employeeplotassignmentHttpController, employeeplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return employeeplotassignmentHandler->erase(request); });

  auto employmentstatusController = std::make_shared<fasc::server::controllers::app::EmploymentStatusController>(database);
  auto employmentstatusHttpController = std::make_shared<fasc::server::controllers::http::EmploymentStatusHttpController>(*employmentstatusController);
  auto employmentstatusHandler = std::make_shared<EmploymentStatusHandler>(*employmentstatusHttpController);
  server.get("/api/employment_status", [employmentstatusController, employmentstatusHttpController, employmentstatusHandler](const fasc::server::core::HttpRequest& request) { return employmentstatusHandler->list(request); });
  server.post("/api/employment_status", [employmentstatusController, employmentstatusHttpController, employmentstatusHandler](const fasc::server::core::HttpRequest& request) { return employmentstatusHandler->create(request); });
  server.get("/api/employment_status/item", [employmentstatusController, employmentstatusHttpController, employmentstatusHandler](const fasc::server::core::HttpRequest& request) { return employmentstatusHandler->load(request); });
  server.put("/api/employment_status/item", [employmentstatusController, employmentstatusHttpController, employmentstatusHandler](const fasc::server::core::HttpRequest& request) { return employmentstatusHandler->update(request); });
  server.del("/api/employment_status/item", [employmentstatusController, employmentstatusHttpController, employmentstatusHandler](const fasc::server::core::HttpRequest& request) { return employmentstatusHandler->erase(request); });

  auto farmController = std::make_shared<fasc::server::controllers::app::FarmController>(database);
  auto farmHttpController = std::make_shared<fasc::server::controllers::http::FarmHttpController>(*farmController);
  auto farmHandler = std::make_shared<FarmHandler>(*farmHttpController);
  server.get("/api/farm", [farmController, farmHttpController, farmHandler](const fasc::server::core::HttpRequest& request) { return farmHandler->list(request); });
  server.post("/api/farm", [farmController, farmHttpController, farmHandler](const fasc::server::core::HttpRequest& request) { return farmHandler->create(request); });
  server.get("/api/farm/item", [farmController, farmHttpController, farmHandler](const fasc::server::core::HttpRequest& request) { return farmHandler->load(request); });
  server.put("/api/farm/item", [farmController, farmHttpController, farmHandler](const fasc::server::core::HttpRequest& request) { return farmHandler->update(request); });
  server.del("/api/farm/item", [farmController, farmHttpController, farmHandler](const fasc::server::core::HttpRequest& request) { return farmHandler->erase(request); });

  auto farmassociationController = std::make_shared<fasc::server::controllers::app::FarmAssociationController>(database);
  auto farmassociationHttpController = std::make_shared<fasc::server::controllers::http::FarmAssociationHttpController>(*farmassociationController);
  auto farmassociationHandler = std::make_shared<FarmAssociationHandler>(*farmassociationHttpController);
  server.get("/api/farm_association", [farmassociationController, farmassociationHttpController, farmassociationHandler](const fasc::server::core::HttpRequest& request) { return farmassociationHandler->list(request); });
  server.post("/api/farm_association", [farmassociationController, farmassociationHttpController, farmassociationHandler](const fasc::server::core::HttpRequest& request) { return farmassociationHandler->create(request); });
  server.get("/api/farm_association/item", [farmassociationController, farmassociationHttpController, farmassociationHandler](const fasc::server::core::HttpRequest& request) { return farmassociationHandler->load(request); });
  server.put("/api/farm_association/item", [farmassociationController, farmassociationHttpController, farmassociationHandler](const fasc::server::core::HttpRequest& request) { return farmassociationHandler->update(request); });
  server.del("/api/farm_association/item", [farmassociationController, farmassociationHttpController, farmassociationHandler](const fasc::server::core::HttpRequest& request) { return farmassociationHandler->erase(request); });

  auto farmemployeeController = std::make_shared<fasc::server::controllers::app::FarmEmployeeController>(database);
  auto farmemployeeHttpController = std::make_shared<fasc::server::controllers::http::FarmEmployeeHttpController>(*farmemployeeController);
  auto farmemployeeHandler = std::make_shared<FarmEmployeeHandler>(*farmemployeeHttpController);
  server.get("/api/farm_employee", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->list(request); });
  server.post("/api/farm_employee", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->create(request); });
  server.get("/api/farm_employee/item", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->load(request); });
  server.put("/api/farm_employee/item", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->update(request); });
  server.del("/api/farm_employee/item", [farmemployeeController, farmemployeeHttpController, farmemployeeHandler](const fasc::server::core::HttpRequest& request) { return farmemployeeHandler->erase(request); });

  auto farmownerController = std::make_shared<fasc::server::controllers::app::FarmOwnerController>(database);
  auto farmownerHttpController = std::make_shared<fasc::server::controllers::http::FarmOwnerHttpController>(*farmownerController);
  auto farmownerHandler = std::make_shared<FarmOwnerHandler>(*farmownerHttpController);
  server.get("/api/farm_owner", [farmownerController, farmownerHttpController, farmownerHandler](const fasc::server::core::HttpRequest& request) { return farmownerHandler->list(request); });
  server.post("/api/farm_owner", [farmownerController, farmownerHttpController, farmownerHandler](const fasc::server::core::HttpRequest& request) { return farmownerHandler->create(request); });
  server.get("/api/farm_owner/item", [farmownerController, farmownerHttpController, farmownerHandler](const fasc::server::core::HttpRequest& request) { return farmownerHandler->load(request); });
  server.put("/api/farm_owner/item", [farmownerController, farmownerHttpController, farmownerHandler](const fasc::server::core::HttpRequest& request) { return farmownerHandler->update(request); });
  server.del("/api/farm_owner/item", [farmownerController, farmownerHttpController, farmownerHandler](const fasc::server::core::HttpRequest& request) { return farmownerHandler->erase(request); });

  auto farmownershipController = std::make_shared<fasc::server::controllers::app::FarmOwnershipController>(database);
  auto farmownershipHttpController = std::make_shared<fasc::server::controllers::http::FarmOwnershipHttpController>(*farmownershipController);
  auto farmownershipHandler = std::make_shared<FarmOwnershipHandler>(*farmownershipHttpController);
  server.get("/api/farm_ownership", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->list(request); });
  server.post("/api/farm_ownership", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->create(request); });
  server.get("/api/farm_ownership/item", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->load(request); });
  server.put("/api/farm_ownership/item", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->update(request); });
  server.del("/api/farm_ownership/item", [farmownershipController, farmownershipHttpController, farmownershipHandler](const fasc::server::core::HttpRequest& request) { return farmownershipHandler->erase(request); });

  auto farmplotController = std::make_shared<fasc::server::controllers::app::FarmPlotController>(database);
  auto farmplotHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotHttpController>(*farmplotController);
  auto farmplotHandler = std::make_shared<FarmPlotHandler>(*farmplotHttpController);
  server.get("/api/farm_plot", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->list(request); });
  server.post("/api/farm_plot", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->create(request); });
  server.get("/api/farm_plot/item", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->load(request); });
  server.put("/api/farm_plot/item", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->update(request); });
  server.del("/api/farm_plot/item", [farmplotController, farmplotHttpController, farmplotHandler](const fasc::server::core::HttpRequest& request) { return farmplotHandler->erase(request); });

  auto farmplotassignmentController = std::make_shared<fasc::server::controllers::app::FarmPlotAssignmentController>(database);
  auto farmplotassignmentHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotAssignmentHttpController>(*farmplotassignmentController);
  auto farmplotassignmentHandler = std::make_shared<FarmPlotAssignmentHandler>(*farmplotassignmentHttpController);
  server.get("/api/farm_plot_assignment", [farmplotassignmentController, farmplotassignmentHttpController, farmplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return farmplotassignmentHandler->list(request); });
  server.post("/api/farm_plot_assignment", [farmplotassignmentController, farmplotassignmentHttpController, farmplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return farmplotassignmentHandler->create(request); });
  server.get("/api/farm_plot_assignment/item", [farmplotassignmentController, farmplotassignmentHttpController, farmplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return farmplotassignmentHandler->load(request); });
  server.put("/api/farm_plot_assignment/item", [farmplotassignmentController, farmplotassignmentHttpController, farmplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return farmplotassignmentHandler->update(request); });
  server.del("/api/farm_plot_assignment/item", [farmplotassignmentController, farmplotassignmentHttpController, farmplotassignmentHandler](const fasc::server::core::HttpRequest& request) { return farmplotassignmentHandler->erase(request); });

  auto farmplotconsumptionproductController = std::make_shared<fasc::server::controllers::app::FarmPlotConsumptionProductController>(database);
  auto farmplotconsumptionproductHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotConsumptionProductHttpController>(*farmplotconsumptionproductController);
  auto farmplotconsumptionproductHandler = std::make_shared<FarmPlotConsumptionProductHandler>(*farmplotconsumptionproductHttpController);
  server.get("/api/farm_plot_consumption_product", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->list(request); });
  server.post("/api/farm_plot_consumption_product", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->create(request); });
  server.get("/api/farm_plot_consumption_product/item", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->load(request); });
  server.put("/api/farm_plot_consumption_product/item", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->update(request); });
  server.del("/api/farm_plot_consumption_product/item", [farmplotconsumptionproductController, farmplotconsumptionproductHttpController, farmplotconsumptionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotconsumptionproductHandler->erase(request); });

  auto farmplotproductionproductController = std::make_shared<fasc::server::controllers::app::FarmPlotProductionProductController>(database);
  auto farmplotproductionproductHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotProductionProductHttpController>(*farmplotproductionproductController);
  auto farmplotproductionproductHandler = std::make_shared<FarmPlotProductionProductHandler>(*farmplotproductionproductHttpController);
  server.get("/api/farm_plot_production_product", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->list(request); });
  server.post("/api/farm_plot_production_product", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->create(request); });
  server.get("/api/farm_plot_production_product/item", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->load(request); });
  server.put("/api/farm_plot_production_product/item", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->update(request); });
  server.del("/api/farm_plot_production_product/item", [farmplotproductionproductController, farmplotproductionproductHttpController, farmplotproductionproductHandler](const fasc::server::core::HttpRequest& request) { return farmplotproductionproductHandler->erase(request); });

  auto farmplottypeController = std::make_shared<fasc::server::controllers::app::FarmPlotTypeController>(database);
  auto farmplottypeHttpController = std::make_shared<fasc::server::controllers::http::FarmPlotTypeHttpController>(*farmplottypeController);
  auto farmplottypeHandler = std::make_shared<FarmPlotTypeHandler>(*farmplottypeHttpController);
  server.get("/api/farm_plot_type", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->list(request); });
  server.post("/api/farm_plot_type", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->create(request); });
  server.get("/api/farm_plot_type/item", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->load(request); });
  server.put("/api/farm_plot_type/item", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->update(request); });
  server.del("/api/farm_plot_type/item", [farmplottypeController, farmplottypeHttpController, farmplottypeHandler](const fasc::server::core::HttpRequest& request) { return farmplottypeHandler->erase(request); });

  auto farmroleController = std::make_shared<fasc::server::controllers::app::FarmRoleController>(database);
  auto farmroleHttpController = std::make_shared<fasc::server::controllers::http::FarmRoleHttpController>(*farmroleController);
  auto farmroleHandler = std::make_shared<FarmRoleHandler>(*farmroleHttpController);
  server.get("/api/farm_role", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->list(request); });
  server.post("/api/farm_role", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->create(request); });
  server.get("/api/farm_role/item", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->load(request); });
  server.put("/api/farm_role/item", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->update(request); });
  server.del("/api/farm_role/item", [farmroleController, farmroleHttpController, farmroleHandler](const fasc::server::core::HttpRequest& request) { return farmroleHandler->erase(request); });

  auto identitydocumenttypeController = std::make_shared<fasc::server::controllers::app::IdentityDocumentTypeController>(database);
  auto identitydocumenttypeHttpController = std::make_shared<fasc::server::controllers::http::IdentityDocumentTypeHttpController>(*identitydocumenttypeController);
  auto identitydocumenttypeHandler = std::make_shared<IdentityDocumentTypeHandler>(*identitydocumenttypeHttpController);
  server.get("/api/identity_document_type", [identitydocumenttypeController, identitydocumenttypeHttpController, identitydocumenttypeHandler](const fasc::server::core::HttpRequest& request) { return identitydocumenttypeHandler->list(request); });
  server.post("/api/identity_document_type", [identitydocumenttypeController, identitydocumenttypeHttpController, identitydocumenttypeHandler](const fasc::server::core::HttpRequest& request) { return identitydocumenttypeHandler->create(request); });
  server.get("/api/identity_document_type/item", [identitydocumenttypeController, identitydocumenttypeHttpController, identitydocumenttypeHandler](const fasc::server::core::HttpRequest& request) { return identitydocumenttypeHandler->load(request); });
  server.put("/api/identity_document_type/item", [identitydocumenttypeController, identitydocumenttypeHttpController, identitydocumenttypeHandler](const fasc::server::core::HttpRequest& request) { return identitydocumenttypeHandler->update(request); });
  server.del("/api/identity_document_type/item", [identitydocumenttypeController, identitydocumenttypeHttpController, identitydocumenttypeHandler](const fasc::server::core::HttpRequest& request) { return identitydocumenttypeHandler->erase(request); });

  auto personController = std::make_shared<fasc::server::controllers::app::PersonController>(database);
  auto personHttpController = std::make_shared<fasc::server::controllers::http::PersonHttpController>(*personController);
  auto personHandler = std::make_shared<PersonHandler>(*personHttpController);
  server.get("/api/person", [personController, personHttpController, personHandler](const fasc::server::core::HttpRequest& request) { return personHandler->list(request); });
  server.post("/api/person", [personController, personHttpController, personHandler](const fasc::server::core::HttpRequest& request) { return personHandler->create(request); });
  server.get("/api/person/item", [personController, personHttpController, personHandler](const fasc::server::core::HttpRequest& request) { return personHandler->load(request); });
  server.put("/api/person/item", [personController, personHttpController, personHandler](const fasc::server::core::HttpRequest& request) { return personHandler->update(request); });
  server.del("/api/person/item", [personController, personHttpController, personHandler](const fasc::server::core::HttpRequest& request) { return personHandler->erase(request); });

  auto persondocumentController = std::make_shared<fasc::server::controllers::app::PersonDocumentController>(database);
  auto persondocumentHttpController = std::make_shared<fasc::server::controllers::http::PersonDocumentHttpController>(*persondocumentController);
  auto persondocumentHandler = std::make_shared<PersonDocumentHandler>(*persondocumentHttpController);
  server.get("/api/person_document", [persondocumentController, persondocumentHttpController, persondocumentHandler](const fasc::server::core::HttpRequest& request) { return persondocumentHandler->list(request); });
  server.post("/api/person_document", [persondocumentController, persondocumentHttpController, persondocumentHandler](const fasc::server::core::HttpRequest& request) { return persondocumentHandler->create(request); });
  server.get("/api/person_document/item", [persondocumentController, persondocumentHttpController, persondocumentHandler](const fasc::server::core::HttpRequest& request) { return persondocumentHandler->load(request); });
  server.put("/api/person_document/item", [persondocumentController, persondocumentHttpController, persondocumentHandler](const fasc::server::core::HttpRequest& request) { return persondocumentHandler->update(request); });
  server.del("/api/person_document/item", [persondocumentController, persondocumentHttpController, persondocumentHandler](const fasc::server::core::HttpRequest& request) { return persondocumentHandler->erase(request); });

  auto productController = std::make_shared<fasc::server::controllers::app::ProductController>(database);
  auto productHttpController = std::make_shared<fasc::server::controllers::http::ProductHttpController>(*productController);
  auto productHandler = std::make_shared<ProductHandler>(*productHttpController);
  server.get("/api/product", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->list(request); });
  server.post("/api/product", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->create(request); });
  server.get("/api/product/item", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->load(request); });
  server.put("/api/product/item", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->update(request); });
  server.del("/api/product/item", [productController, productHttpController, productHandler](const fasc::server::core::HttpRequest& request) { return productHandler->erase(request); });

  auto producttypeController = std::make_shared<fasc::server::controllers::app::ProductTypeController>(database);
  auto producttypeHttpController = std::make_shared<fasc::server::controllers::http::ProductTypeHttpController>(*producttypeController);
  auto producttypeHandler = std::make_shared<ProductTypeHandler>(*producttypeHttpController);
  server.get("/api/product_type", [producttypeController, producttypeHttpController, producttypeHandler](const fasc::server::core::HttpRequest& request) { return producttypeHandler->list(request); });
  server.post("/api/product_type", [producttypeController, producttypeHttpController, producttypeHandler](const fasc::server::core::HttpRequest& request) { return producttypeHandler->create(request); });
  server.get("/api/product_type/item", [producttypeController, producttypeHttpController, producttypeHandler](const fasc::server::core::HttpRequest& request) { return producttypeHandler->load(request); });
  server.put("/api/product_type/item", [producttypeController, producttypeHttpController, producttypeHandler](const fasc::server::core::HttpRequest& request) { return producttypeHandler->update(request); });
  server.del("/api/product_type/item", [producttypeController, producttypeHttpController, producttypeHandler](const fasc::server::core::HttpRequest& request) { return producttypeHandler->erase(request); });

  auto purchaseorderController = std::make_shared<fasc::server::controllers::app::PurchaseOrderController>(database);
  auto purchaseorderHttpController = std::make_shared<fasc::server::controllers::http::PurchaseOrderHttpController>(*purchaseorderController);
  auto purchaseorderHandler = std::make_shared<PurchaseOrderHandler>(*purchaseorderHttpController);
  server.get("/api/purchase_order", [purchaseorderController, purchaseorderHttpController, purchaseorderHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderHandler->list(request); });
  server.post("/api/purchase_order", [purchaseorderController, purchaseorderHttpController, purchaseorderHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderHandler->create(request); });
  server.get("/api/purchase_order/item", [purchaseorderController, purchaseorderHttpController, purchaseorderHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderHandler->load(request); });
  server.put("/api/purchase_order/item", [purchaseorderController, purchaseorderHttpController, purchaseorderHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderHandler->update(request); });
  server.del("/api/purchase_order/item", [purchaseorderController, purchaseorderHttpController, purchaseorderHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderHandler->erase(request); });

  auto purchaseorderitemController = std::make_shared<fasc::server::controllers::app::PurchaseOrderItemController>(database);
  auto purchaseorderitemHttpController = std::make_shared<fasc::server::controllers::http::PurchaseOrderItemHttpController>(*purchaseorderitemController);
  auto purchaseorderitemHandler = std::make_shared<PurchaseOrderItemHandler>(*purchaseorderitemHttpController);
  server.get("/api/purchase_order_item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->list(request); });
  server.post("/api/purchase_order_item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->create(request); });
  server.get("/api/purchase_order_item/item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->load(request); });
  server.put("/api/purchase_order_item/item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->update(request); });
  server.del("/api/purchase_order_item/item", [purchaseorderitemController, purchaseorderitemHttpController, purchaseorderitemHandler](const fasc::server::core::HttpRequest& request) { return purchaseorderitemHandler->erase(request); });

  auto purchaserequisitionController = std::make_shared<fasc::server::controllers::app::PurchaseRequisitionController>(database);
  auto purchaserequisitionHttpController = std::make_shared<fasc::server::controllers::http::PurchaseRequisitionHttpController>(*purchaserequisitionController);
  auto purchaserequisitionHandler = std::make_shared<PurchaseRequisitionHandler>(*purchaserequisitionHttpController);
  server.get("/api/purchase_requisition", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->list(request); });
  server.post("/api/purchase_requisition", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->create(request); });
  server.get("/api/purchase_requisition/item", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->load(request); });
  server.put("/api/purchase_requisition/item", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->update(request); });
  server.del("/api/purchase_requisition/item", [purchaserequisitionController, purchaserequisitionHttpController, purchaserequisitionHandler](const fasc::server::core::HttpRequest& request) { return purchaserequisitionHandler->erase(request); });

  auto salesrequisitionController = std::make_shared<fasc::server::controllers::app::SalesRequisitionController>(database);
  auto salesrequisitionHttpController = std::make_shared<fasc::server::controllers::http::SalesRequisitionHttpController>(*salesrequisitionController);
  auto salesrequisitionHandler = std::make_shared<SalesRequisitionHandler>(*salesrequisitionHttpController);
  server.get("/api/sales_requisition", [salesrequisitionController, salesrequisitionHttpController, salesrequisitionHandler](const fasc::server::core::HttpRequest& request) { return salesrequisitionHandler->list(request); });
  server.post("/api/sales_requisition", [salesrequisitionController, salesrequisitionHttpController, salesrequisitionHandler](const fasc::server::core::HttpRequest& request) { return salesrequisitionHandler->create(request); });
  server.get("/api/sales_requisition/item", [salesrequisitionController, salesrequisitionHttpController, salesrequisitionHandler](const fasc::server::core::HttpRequest& request) { return salesrequisitionHandler->load(request); });
  server.put("/api/sales_requisition/item", [salesrequisitionController, salesrequisitionHttpController, salesrequisitionHandler](const fasc::server::core::HttpRequest& request) { return salesrequisitionHandler->update(request); });
  server.del("/api/sales_requisition/item", [salesrequisitionController, salesrequisitionHttpController, salesrequisitionHandler](const fasc::server::core::HttpRequest& request) { return salesrequisitionHandler->erase(request); });

  auto supplierController = std::make_shared<fasc::server::controllers::app::SupplierController>(database);
  auto supplierHttpController = std::make_shared<fasc::server::controllers::http::SupplierHttpController>(*supplierController);
  auto supplierHandler = std::make_shared<SupplierHandler>(*supplierHttpController);
  server.get("/api/supplier", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->list(request); });
  server.post("/api/supplier", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->create(request); });
  server.get("/api/supplier/item", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->load(request); });
  server.put("/api/supplier/item", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->update(request); });
  server.del("/api/supplier/item", [supplierController, supplierHttpController, supplierHandler](const fasc::server::core::HttpRequest& request) { return supplierHandler->erase(request); });

  auto supplierproductpriceController = std::make_shared<fasc::server::controllers::app::SupplierProductPriceController>(database);
  auto supplierproductpriceHttpController = std::make_shared<fasc::server::controllers::http::SupplierProductPriceHttpController>(*supplierproductpriceController);
  auto supplierproductpriceHandler = std::make_shared<SupplierProductPriceHandler>(*supplierproductpriceHttpController);
  server.get("/api/supplier_product_price", [supplierproductpriceController, supplierproductpriceHttpController, supplierproductpriceHandler](const fasc::server::core::HttpRequest& request) { return supplierproductpriceHandler->list(request); });
  server.post("/api/supplier_product_price", [supplierproductpriceController, supplierproductpriceHttpController, supplierproductpriceHandler](const fasc::server::core::HttpRequest& request) { return supplierproductpriceHandler->create(request); });
  server.get("/api/supplier_product_price/item", [supplierproductpriceController, supplierproductpriceHttpController, supplierproductpriceHandler](const fasc::server::core::HttpRequest& request) { return supplierproductpriceHandler->load(request); });
  server.put("/api/supplier_product_price/item", [supplierproductpriceController, supplierproductpriceHttpController, supplierproductpriceHandler](const fasc::server::core::HttpRequest& request) { return supplierproductpriceHandler->update(request); });
  server.del("/api/supplier_product_price/item", [supplierproductpriceController, supplierproductpriceHttpController, supplierproductpriceHandler](const fasc::server::core::HttpRequest& request) { return supplierproductpriceHandler->erase(request); });

  auto unitController = std::make_shared<fasc::server::controllers::app::UnitController>(database);
  auto unitHttpController = std::make_shared<fasc::server::controllers::http::UnitHttpController>(*unitController);
  auto unitHandler = std::make_shared<UnitHandler>(*unitHttpController);
  server.get("/api/unit", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->list(request); });
  server.post("/api/unit", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->create(request); });
  server.get("/api/unit/item", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->load(request); });
  server.put("/api/unit/item", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->update(request); });
  server.del("/api/unit/item", [unitController, unitHttpController, unitHandler](const fasc::server::core::HttpRequest& request) { return unitHandler->erase(request); });

}

} // namespace fasc::server::handling
