#include <controllers/app/Users.hpp>
#include <controllers/http/Users.hpp>
#include <database/database.hpp>
#include <database/PgConnection.hpp>
#include <handling/Health.hpp>
#include <handling/tables/AssociationEmployee.hpp>
#include <handling/tables/AssociationFarms.hpp>
#include <handling/tables/AssociationMember.hpp>
#include <handling/tables/AssociationRole.hpp>
#include <handling/tables/Contract.hpp>
#include <handling/tables/EmployeePlotAssignment.hpp>
#include <handling/tables/EmploymentStatus.hpp>
#include <handling/tables/Farm.hpp>
#include <handling/tables/FarmAssociation.hpp>
#include <handling/tables/FarmEmployee.hpp>
#include <handling/tables/FarmOwner.hpp>
#include <handling/tables/FarmOwnership.hpp>
#include <handling/tables/FarmPlot.hpp>
#include <handling/tables/FarmPlotAssignment.hpp>
#include <handling/tables/FarmPlotConsumptionProduct.hpp>
#include <handling/tables/FarmPlotProductionProduct.hpp>
#include <handling/tables/FarmPlotType.hpp>
#include <handling/tables/FarmRole.hpp>
#include <handling/tables/IdentityDocumentType.hpp>
#include <handling/tables/Person.hpp>
#include <handling/tables/PersonDocument.hpp>
#include <handling/tables/Product.hpp>
#include <handling/tables/ProductType.hpp>
#include <handling/tables/PurchaseOrder.hpp>
#include <handling/tables/PurchaseOrderItem.hpp>
#include <handling/tables/PurchaseRequisition.hpp>
#include <handling/tables/SalesRequisition.hpp>
#include <handling/tables/Supplier.hpp>
#include <handling/tables/SupplierProductPrice.hpp>
#include <handling/tables/Unit.hpp>
#include <handling/Users.hpp>
#include <security/JwtService.hpp>
#include <security/PasswordHasher.hpp>
#include <server/core/Server.hpp>

#include <fmt/core.h>

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kServerName = "farm-association-server";
constexpr std::string_view kServerVersion = "0.1.0";
constexpr std::string_view kDefaultJwtSecret = "development-secret-change-me";

void print_version() {
  std::cout << fmt::format("{} version {}\n", kServerName, kServerVersion);
}

std::string env_or(const char* name, std::string fallback) {
  if (const char* value = std::getenv(name)) {
    return value;
  }

  return fallback;
}

template <typename Value>
Value parse_number(std::string_view value, std::string_view setting_name) {
  if (value.empty() || value.front() == '-') {
    throw std::invalid_argument{std::string{setting_name} + " must be a positive number"};
  }

  std::size_t parsed = 0;
  const auto number = std::stoull(std::string{value}, &parsed);
  if (parsed != value.size()) {
    throw std::invalid_argument{std::string{setting_name} + " must be a number"};
  }

  if (number > static_cast<unsigned long long>(std::numeric_limits<Value>::max())) {
    throw std::out_of_range{std::string{setting_name} + " is out of range"};
  }

  return static_cast<Value>(number);
}

ServerSettings settings_from_env() {
  ServerSettings settings;
  settings.address = env_or("FARM_SERVER_ADDRESS", settings.address);

  if (const char* value = std::getenv("FARM_SERVER_PORT")) {
    settings.port = parse_number<unsigned short>(value, "FARM_SERVER_PORT");
  }

  if (const char* value = std::getenv("FARM_SERVER_THREADS")) {
    settings.thread_count = parse_number<std::size_t>(value, "FARM_SERVER_THREADS");
  }

  if (const char* value = std::getenv("FARM_SERVER_BODY_LIMIT")) {
    settings.request_body_limit = parse_number<std::size_t>(value, "FARM_SERVER_BODY_LIMIT");
  }

  if (const char* value = std::getenv("FARM_SERVER_TIMEOUT_SECONDS")) {
    settings.request_timeout =
        std::chrono::seconds{parse_number<long long>(value, "FARM_SERVER_TIMEOUT_SECONDS")};
  }

  if (const char* value = std::getenv("FARM_SERVER_LISTEN_BACKLOG")) {
    settings.listen_backlog = parse_number<int>(value, "FARM_SERVER_LISTEN_BACKLOG");
  }

  return settings;
}

void apply_cli_arg(ServerSettings& settings, std::string_view option, std::string_view value) {
  if (option == "--address") {
    settings.address = value;
  } else if (option == "--port") {
    settings.port = parse_number<unsigned short>(value, "--port");
  } else if (option == "--threads") {
    settings.thread_count = parse_number<std::size_t>(value, "--threads");
  } else if (option == "--body-limit") {
    settings.request_body_limit = parse_number<std::size_t>(value, "--body-limit");
  } else if (option == "--timeout") {
    settings.request_timeout = std::chrono::seconds{parse_number<long long>(value, "--timeout")};
  } else if (option == "--backlog") {
    settings.listen_backlog = parse_number<int>(value, "--backlog");
  } else {
    throw std::invalid_argument{std::string{"Unknown option: "} + std::string{option}};
  }
}

ServerSettings settings_from_env_and_cli(int argc, char* argv[]) {
  ServerSettings settings = settings_from_env();

  for (int i = 1; i < argc; ++i) {
    const std::string_view option{argv[i]};
    if (option == "--version") {
      continue;
    }

    if (i + 1 >= argc) {
      throw std::invalid_argument{std::string{"Missing value for option: "} +
                                  std::string{option}};
    }

    apply_cli_arg(settings, option, argv[++i]);
  }

  return settings;
}

} // namespace

int main(int argc, char* argv[]) {
  if (argc > 1 && std::string_view{argv[1]} == "--version") {
    print_version();
    return 0;
  }

  const ServerSettings settings = settings_from_env_and_cli(argc, argv);

  auto database = fasc::server::database::Database::createFromEnv();
  fasc::server::database::PgConnection table_records_database;
  AssociationEmployeePersistence association_employee_persistence{table_records_database};
  AssociationEmployeeController association_employee_controller{association_employee_persistence};
  AssociationEmployeeHandler association_employee_handler{association_employee_controller};
  AssociationFarmsPersistence association_farms_persistence{table_records_database};
  AssociationFarmsController association_farms_controller{association_farms_persistence};
  AssociationFarmsHandler association_farms_handler{association_farms_controller};
  AssociationMemberPersistence association_member_persistence{table_records_database};
  AssociationMemberController association_member_controller{association_member_persistence};
  AssociationMemberHandler association_member_handler{association_member_controller};
  AssociationRolePersistence association_role_persistence{table_records_database};
  AssociationRoleController association_role_controller{association_role_persistence};
  AssociationRoleHandler association_role_handler{association_role_controller};
  ContractPersistence contract_persistence{table_records_database};
  ContractController contract_controller{contract_persistence};
  ContractHandler contract_handler{contract_controller};
  EmployeePlotAssignmentPersistence employee_plot_assignment_persistence{table_records_database};
  EmployeePlotAssignmentController employee_plot_assignment_controller{employee_plot_assignment_persistence};
  EmployeePlotAssignmentHandler employee_plot_assignment_handler{employee_plot_assignment_controller};
  EmploymentStatusPersistence employment_status_persistence{table_records_database};
  EmploymentStatusController employment_status_controller{employment_status_persistence};
  EmploymentStatusHandler employment_status_handler{employment_status_controller};
  FarmPersistence farm_persistence{table_records_database};
  FarmController farm_controller{farm_persistence};
  FarmHandler farm_handler{farm_controller};
  FarmAssociationPersistence farm_association_persistence{table_records_database};
  FarmAssociationController farm_association_controller{farm_association_persistence};
  FarmAssociationHandler farm_association_handler{farm_association_controller};
  FarmEmployeePersistence farm_employee_persistence{table_records_database};
  FarmEmployeeController farm_employee_controller{farm_employee_persistence};
  FarmEmployeeHandler farm_employee_handler{farm_employee_controller};
  FarmOwnerPersistence farm_owner_persistence{table_records_database};
  FarmOwnerController farm_owner_controller{farm_owner_persistence};
  FarmOwnerHandler farm_owner_handler{farm_owner_controller};
  FarmOwnershipPersistence farm_ownership_persistence{table_records_database};
  FarmOwnershipController farm_ownership_controller{farm_ownership_persistence};
  FarmOwnershipHandler farm_ownership_handler{farm_ownership_controller};
  FarmPlotPersistence farm_plot_persistence{table_records_database};
  FarmPlotController farm_plot_controller{farm_plot_persistence};
  FarmPlotHandler farm_plot_handler{farm_plot_controller};
  FarmPlotAssignmentPersistence farm_plot_assignment_persistence{table_records_database};
  FarmPlotAssignmentController farm_plot_assignment_controller{farm_plot_assignment_persistence};
  FarmPlotAssignmentHandler farm_plot_assignment_handler{farm_plot_assignment_controller};
  FarmPlotConsumptionProductPersistence farm_plot_consumption_product_persistence{table_records_database};
  FarmPlotConsumptionProductController farm_plot_consumption_product_controller{farm_plot_consumption_product_persistence};
  FarmPlotConsumptionProductHandler farm_plot_consumption_product_handler{farm_plot_consumption_product_controller};
  FarmPlotProductionProductPersistence farm_plot_production_product_persistence{table_records_database};
  FarmPlotProductionProductController farm_plot_production_product_controller{farm_plot_production_product_persistence};
  FarmPlotProductionProductHandler farm_plot_production_product_handler{farm_plot_production_product_controller};
  FarmPlotTypePersistence farm_plot_type_persistence{table_records_database};
  FarmPlotTypeController farm_plot_type_controller{farm_plot_type_persistence};
  FarmPlotTypeHandler farm_plot_type_handler{farm_plot_type_controller};
  FarmRolePersistence farm_role_persistence{table_records_database};
  FarmRoleController farm_role_controller{farm_role_persistence};
  FarmRoleHandler farm_role_handler{farm_role_controller};
  IdentityDocumentTypePersistence identity_document_type_persistence{table_records_database};
  IdentityDocumentTypeController identity_document_type_controller{identity_document_type_persistence};
  IdentityDocumentTypeHandler identity_document_type_handler{identity_document_type_controller};
  PersonPersistence person_persistence{table_records_database};
  PersonController person_controller{person_persistence};
  PersonHandler person_handler{person_controller};
  PersonDocumentPersistence person_document_persistence{table_records_database};
  PersonDocumentController person_document_controller{person_document_persistence};
  PersonDocumentHandler person_document_handler{person_document_controller};
  ProductPersistence product_persistence{table_records_database};
  ProductController product_controller{product_persistence};
  ProductHandler product_handler{product_controller};
  ProductTypePersistence product_type_persistence{table_records_database};
  ProductTypeController product_type_controller{product_type_persistence};
  ProductTypeHandler product_type_handler{product_type_controller};
  PurchaseOrderPersistence purchase_order_persistence{table_records_database};
  PurchaseOrderController purchase_order_controller{purchase_order_persistence};
  PurchaseOrderHandler purchase_order_handler{purchase_order_controller};
  PurchaseOrderItemPersistence purchase_order_item_persistence{table_records_database};
  PurchaseOrderItemController purchase_order_item_controller{purchase_order_item_persistence};
  PurchaseOrderItemHandler purchase_order_item_handler{purchase_order_item_controller};
  PurchaseRequisitionPersistence purchase_requisition_persistence{table_records_database};
  PurchaseRequisitionController purchase_requisition_controller{purchase_requisition_persistence};
  PurchaseRequisitionHandler purchase_requisition_handler{purchase_requisition_controller};
  SalesRequisitionPersistence sales_requisition_persistence{table_records_database};
  SalesRequisitionController sales_requisition_controller{sales_requisition_persistence};
  SalesRequisitionHandler sales_requisition_handler{sales_requisition_controller};
  SupplierPersistence supplier_persistence{table_records_database};
  SupplierController supplier_controller{supplier_persistence};
  SupplierHandler supplier_handler{supplier_controller};
  SupplierProductPricePersistence supplier_product_price_persistence{table_records_database};
  SupplierProductPriceController supplier_product_price_controller{supplier_product_price_persistence};
  SupplierProductPriceHandler supplier_product_price_handler{supplier_product_price_controller};
  UnitPersistence unit_persistence{table_records_database};
  UnitController unit_controller{unit_persistence};
  UnitHandler unit_handler{unit_controller};
  fasc::server::security::PasswordHasher password_hasher;
  fasc::server::security::JwtService jwt_service{
      env_or("FARM_JWT_SECRET", std::string{kDefaultJwtSecret})};

  UserController user_controller{database, password_hasher, jwt_service};
  UserHttpController user_http_controller{user_controller};

  HealthHandler health_handler;
  UserHandler user_handler{user_http_controller};

  Server server;
  server.get("/health", [&](const HttpRequest& request) { return health_handler.health(request); });
  server.post("/auth/register",
              [&](const HttpRequest& request) { return user_handler.register_user(request); });
  server.post("/auth/login",
              [&](const HttpRequest& request) { return user_handler.login_user(request); });
  server.post("/users",
              [&](const HttpRequest& request) { return user_handler.create_user(request); });
  registerAssociationEmployeeRoutes(server, association_employee_handler);
  registerAssociationFarmsRoutes(server, association_farms_handler);
  registerAssociationMemberRoutes(server, association_member_handler);
  registerAssociationRoleRoutes(server, association_role_handler);
  registerContractRoutes(server, contract_handler);
  registerEmployeePlotAssignmentRoutes(server, employee_plot_assignment_handler);
  registerEmploymentStatusRoutes(server, employment_status_handler);
  registerFarmRoutes(server, farm_handler);
  registerFarmAssociationRoutes(server, farm_association_handler);
  registerFarmEmployeeRoutes(server, farm_employee_handler);
  registerFarmOwnerRoutes(server, farm_owner_handler);
  registerFarmOwnershipRoutes(server, farm_ownership_handler);
  registerFarmPlotRoutes(server, farm_plot_handler);
  registerFarmPlotAssignmentRoutes(server, farm_plot_assignment_handler);
  registerFarmPlotConsumptionProductRoutes(server, farm_plot_consumption_product_handler);
  registerFarmPlotProductionProductRoutes(server, farm_plot_production_product_handler);
  registerFarmPlotTypeRoutes(server, farm_plot_type_handler);
  registerFarmRoleRoutes(server, farm_role_handler);
  registerIdentityDocumentTypeRoutes(server, identity_document_type_handler);
  registerPersonRoutes(server, person_handler);
  registerPersonDocumentRoutes(server, person_document_handler);
  registerProductRoutes(server, product_handler);
  registerProductTypeRoutes(server, product_type_handler);
  registerPurchaseOrderRoutes(server, purchase_order_handler);
  registerPurchaseOrderItemRoutes(server, purchase_order_item_handler);
  registerPurchaseRequisitionRoutes(server, purchase_requisition_handler);
  registerSalesRequisitionRoutes(server, sales_requisition_handler);
  registerSupplierRoutes(server, supplier_handler);
  registerSupplierProductPriceRoutes(server, supplier_product_price_handler);
  registerUnitRoutes(server, unit_handler);

  std::cout << fmt::format("{} starting on {}:{}\n", kServerName, settings.address, settings.port);
  return server.run(settings);
}
