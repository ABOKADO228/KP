#include <database/OdbTableRegistry.hpp>

#include <domain/Types.hpp>

#include <odb/database.hxx>
#include <odb/nullable.hxx>
#include <odb/result.hxx>

#include <persistence/AssociationEmployee.hpp>
#include <persistence/association-employee-odb.hxx>
#include <persistence/AssociationFarms.hpp>
#include <persistence/association-farms-odb.hxx>
#include <persistence/AssociationMember.hpp>
#include <persistence/association-member-odb.hxx>
#include <persistence/AssociationRole.hpp>
#include <persistence/association-role-odb.hxx>
#include <persistence/Contract.hpp>
#include <persistence/contract-odb.hxx>
#include <persistence/EmployeePlotAssignment.hpp>
#include <persistence/employee-plot-assignment-odb.hxx>
#include <persistence/EmploymentStatus.hpp>
#include <persistence/employment-status-odb.hxx>
#include <persistence/Farm.hpp>
#include <persistence/farm-odb.hxx>
#include <persistence/FarmAssociation.hpp>
#include <persistence/farm-association-odb.hxx>
#include <persistence/FarmEmployee.hpp>
#include <persistence/farm-employee-odb.hxx>
#include <persistence/FarmOwner.hpp>
#include <persistence/farm-owner-odb.hxx>
#include <persistence/FarmOwnership.hpp>
#include <persistence/farm-ownership-odb.hxx>
#include <persistence/FarmPlot.hpp>
#include <persistence/farm-plot-odb.hxx>
#include <persistence/FarmPlotAssignment.hpp>
#include <persistence/farm-plot-assignment-odb.hxx>
#include <persistence/FarmPlotConsumptionProduct.hpp>
#include <persistence/farm-plot-consumption-product-odb.hxx>
#include <persistence/FarmPlotProductionProduct.hpp>
#include <persistence/farm-plot-production-product-odb.hxx>
#include <persistence/FarmPlotType.hpp>
#include <persistence/farm-plot-type-odb.hxx>
#include <persistence/FarmRole.hpp>
#include <persistence/farm-role-odb.hxx>
#include <persistence/IdentityDocumentType.hpp>
#include <persistence/identity-document-type-odb.hxx>
#include <persistence/Person.hpp>
#include <persistence/person-odb.hxx>
#include <persistence/PersonDocument.hpp>
#include <persistence/person-document-odb.hxx>
#include <persistence/Product.hpp>
#include <persistence/product-odb.hxx>
#include <persistence/ProductType.hpp>
#include <persistence/product-type-odb.hxx>
#include <persistence/PurchaseOrder.hpp>
#include <persistence/purchase-order-odb.hxx>
#include <persistence/PurchaseOrderItem.hpp>
#include <persistence/purchase-order-item-odb.hxx>
#include <persistence/PurchaseRequisition.hpp>
#include <persistence/purchase-requisition-odb.hxx>
#include <persistence/SalesRequisition.hpp>
#include <persistence/sales-requisition-odb.hxx>
#include <persistence/Supplier.hpp>
#include <persistence/supplier-odb.hxx>
#include <persistence/SupplierProductPrice.hpp>
#include <persistence/supplier-product-price-odb.hxx>
#include <persistence/Unit.hpp>
#include <persistence/unit-odb.hxx>

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace fasc::server::database {
namespace {

template <typename Value>
std::optional<std::string> cellFromValue(const Value& value) {
  return fasc::server::domain::toSqlText(value);
}

template <typename Value>
std::optional<std::string> cellFromValue(const odb::nullable<Value>& value) {
  if (value.null()) {
    return std::nullopt;
  }

  return fasc::server::domain::toSqlText(value.get());
}

template <typename Value>
void assignValue(Value& target, const SqlParameter& parameter, std::string_view column) {
  if (parameter.isNull) {
    throw std::invalid_argument{"Column cannot be null: " + std::string{column}};
  }

  target = fasc::server::domain::parseSqlText<Value>(parameter.text);
}

template <typename Value>
void assignValue(odb::nullable<Value>& target,
                 const SqlParameter& parameter,
                 std::string_view) {
  if (parameter.isNull) {
    target.reset();
    return;
  }

  target = fasc::server::domain::parseSqlText<Value>(parameter.text);
}

template <typename Entity>
struct FieldDescriptor {
  const char* column;
  std::optional<std::string> (*read)(const Entity& entity);
  void (*write)(Entity& entity, const SqlParameter& parameter, std::string_view column);
};

template <typename Entity, auto Member>
std::optional<std::string> readMember(const Entity& entity) {
  return cellFromValue(entity.*Member);
}

template <typename Entity, auto Member>
void writeMember(Entity& entity, const SqlParameter& parameter, std::string_view column) {
  assignValue(entity.*Member, parameter, column);
}

template <typename Entity, auto Member>
FieldDescriptor<Entity> field(const char* column) {
  return FieldDescriptor<Entity>{
      column,
      &readMember<Entity, Member>,
      &writeMember<Entity, Member>};
}

void requireSameSize(std::string_view name,
                     const std::vector<std::string>& columns,
                     const std::vector<SqlParameter>& values) {
  if (columns.size() != values.size()) {
    throw std::invalid_argument{std::string{name} + " columns and values size mismatch"};
  }
}

std::string_view normalizeTableName(std::string_view table) {
  constexpr std::string_view publicPrefix{"public."};
  if (table.substr(0, publicPrefix.size()) == publicPrefix) {
    return table.substr(publicPrefix.size());
  }

  return table;
}

bool cellEqualsParameter(const std::optional<std::string>& cell, const SqlParameter& parameter) {
  if (parameter.isNull) {
    return !cell.has_value();
  }

  return cell.has_value() && *cell == parameter.text;
}

class TableOperationsBase {
public:
  virtual ~TableOperationsBase() = default;

  virtual std::string_view table() const = 0;

  virtual std::vector<SqlRow> select(Database& db,
                                     const std::vector<std::string>& columns) const = 0;

  virtual std::optional<SqlRow> selectOne(
      Database& db,
      const std::vector<std::string>& columns,
      const std::vector<std::string>& keyColumns,
      const std::vector<SqlParameter>& keyValues) const = 0;

  virtual unsigned long long insert(Database& db,
                                    const std::vector<std::string>& columns,
                                    const std::vector<SqlParameter>& values) const = 0;

  virtual unsigned long long update(Database& db,
                                    const std::vector<std::string>& columns,
                                    const std::vector<SqlParameter>& values,
                                    const std::vector<std::string>& keyColumns,
                                    const std::vector<SqlParameter>& keyValues) const = 0;

  virtual unsigned long long erase(Database& db,
                                   const std::vector<std::string>& keyColumns,
                                   const std::vector<SqlParameter>& keyValues) const = 0;
};

template <typename Entity>
class TableOperations final : public TableOperationsBase {
public:
  TableOperations(std::string_view table,
                  std::vector<FieldDescriptor<Entity>> fields,
                  std::vector<std::string> keyColumns)
      : table_(table), fields_(std::move(fields)), keyColumns_(std::move(keyColumns)) {}

  std::string_view table() const override {
    return table_;
  }

  std::vector<SqlRow> select(Database& db,
                             const std::vector<std::string>& columns) const override {
    std::vector<SqlRow> rows;
    for (const Entity& entity : loadAll(db)) {
      rows.push_back(rowFrom(entity, columns));
    }
    return rows;
  }

  std::optional<SqlRow> selectOne(
      Database& db,
      const std::vector<std::string>& columns,
      const std::vector<std::string>& keyColumns,
      const std::vector<SqlParameter>& keyValues) const override {
    requireSameSize("key", keyColumns, keyValues);
    for (const Entity& entity : loadAll(db)) {
      if (matches(entity, keyColumns, keyValues)) {
        return rowFrom(entity, columns);
      }
    }

    return std::nullopt;
  }

  unsigned long long insert(Database& db,
                            const std::vector<std::string>& columns,
                            const std::vector<SqlParameter>& values) const override {
    requireSameSize("insert", columns, values);
    Entity entity{};
    apply(entity, columns, values);
    db.raw().persist(entity);
    return 1;
  }

  unsigned long long update(Database& db,
                            const std::vector<std::string>& columns,
                            const std::vector<SqlParameter>& values,
                            const std::vector<std::string>& keyColumns,
                            const std::vector<SqlParameter>& keyValues) const override {
    requireSameSize("update", columns, values);
    requireSameSize("key", keyColumns, keyValues);

    unsigned long long affectedRows{};
    for (Entity& entity : loadAll(db)) {
      if (matches(entity, keyColumns, keyValues)) {
        apply(entity, columns, values);
        db.raw().update(entity);
        ++affectedRows;
      }
    }

    return affectedRows;
  }

  unsigned long long erase(Database& db,
                           const std::vector<std::string>& keyColumns,
                           const std::vector<SqlParameter>& keyValues) const override {
    requireSameSize("key", keyColumns, keyValues);

    unsigned long long affectedRows{};
    for (Entity& entity : loadAll(db)) {
      if (matches(entity, keyColumns, keyValues)) {
        db.raw().erase(entity);
        ++affectedRows;
      }
    }

    return affectedRows;
  }

private:
  std::vector<Entity> loadAll(Database& db) const {
    odb::result<Entity> result = db.raw().query<Entity>(false);
    std::vector<Entity> entities;
    for (auto iterator = result.begin(); iterator != result.end(); ++iterator) {
      entities.push_back(*iterator);
    }
    return entities;
  }

  const FieldDescriptor<Entity>& requireField(std::string_view column) const {
    const auto it = std::find_if(fields_.begin(), fields_.end(), [&](const auto& field) {
      return std::string_view{field.column} == column;
    });
    if (it == fields_.end()) {
      throw std::invalid_argument{"Unknown column for ODB table " + std::string{table_} +
                                  ": " + std::string{column}};
    }
    return *it;
  }

  SqlRow rowFrom(const Entity& entity, const std::vector<std::string>& columns) const {
    SqlRow row;
    for (const std::string& column : columns) {
      const auto& field = requireField(column);
      row.emplace(column, field.read(entity));
    }
    return row;
  }

  void apply(Entity& entity,
             const std::vector<std::string>& columns,
             const std::vector<SqlParameter>& values) const {
    for (std::size_t index = 0; index < columns.size(); ++index) {
      const auto& field = requireField(columns[index]);
      field.write(entity, values[index], columns[index]);
    }
  }

  bool matches(const Entity& entity,
               const std::vector<std::string>& keyColumns,
               const std::vector<SqlParameter>& keyValues) const {
    if (keyColumns.empty()) {
      throw std::invalid_argument{"At least one key column is required"};
    }

    for (std::size_t index = 0; index < keyColumns.size(); ++index) {
      const auto& field = requireField(keyColumns[index]);
      if (!cellEqualsParameter(field.read(entity), keyValues[index])) {
        return false;
      }
    }

    return true;
  }

  std::string_view table_;
  std::vector<FieldDescriptor<Entity>> fields_;
  std::vector<std::string> keyColumns_;
};

using namespace fasc::server::persistence;

const TableOperations<AssociationEmployeeEntity> kAssociationEmployeeTable{
    "association_employee",
    {
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::id>("id"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::personId>("person_id"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::associationId>("association_id"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::roleId>("role_id"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::employmentStatusId>("employment_status_id"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::hireDate>("hire_date"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::dismissalDate>("dismissal_date"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::salary>("salary"),
      field<AssociationEmployeeEntity, &AssociationEmployeeEntity::contractNumber>("contract_number")
    },
    {"id"}};

const TableOperations<AssociationFarmsEntity> kAssociationFarmsTable{
    "association_farms",
    {
      field<AssociationFarmsEntity, &AssociationFarmsEntity::farmId>("farm_id"),
      field<AssociationFarmsEntity, &AssociationFarmsEntity::associationId>("association_id"),
      field<AssociationFarmsEntity, &AssociationFarmsEntity::joinDate>("join_date"),
      field<AssociationFarmsEntity, &AssociationFarmsEntity::status>("status"),
      field<AssociationFarmsEntity, &AssociationFarmsEntity::notes>("notes")
    },
    {"farm_id", "association_id"}};

const TableOperations<AssociationMemberEntity> kAssociationMemberTable{
    "association_member",
    {
      field<AssociationMemberEntity, &AssociationMemberEntity::id>("id"),
      field<AssociationMemberEntity, &AssociationMemberEntity::associationId>("association_id"),
      field<AssociationMemberEntity, &AssociationMemberEntity::personId>("person_id"),
      field<AssociationMemberEntity, &AssociationMemberEntity::membershipNumber>("membership_number"),
      field<AssociationMemberEntity, &AssociationMemberEntity::joinedDate>("joined_date"),
      field<AssociationMemberEntity, &AssociationMemberEntity::status>("status"),
      field<AssociationMemberEntity, &AssociationMemberEntity::notes>("notes")
    },
    {"id"}};

const TableOperations<AssociationRoleEntity> kAssociationRoleTable{
    "association_role",
    {
      field<AssociationRoleEntity, &AssociationRoleEntity::id>("id"),
      field<AssociationRoleEntity, &AssociationRoleEntity::code>("code"),
      field<AssociationRoleEntity, &AssociationRoleEntity::name>("name"),
      field<AssociationRoleEntity, &AssociationRoleEntity::description>("description")
    },
    {"id"}};

const TableOperations<ContractEntity> kContractTable{
    "contract",
    {
      field<ContractEntity, &ContractEntity::id>("id"),
      field<ContractEntity, &ContractEntity::supplierId>("supplier_id"),
      field<ContractEntity, &ContractEntity::farmId>("farm_id"),
      field<ContractEntity, &ContractEntity::associationId>("association_id"),
      field<ContractEntity, &ContractEntity::contractNumber>("contract_number"),
      field<ContractEntity, &ContractEntity::signDate>("sign_date"),
      field<ContractEntity, &ContractEntity::startDate>("start_date"),
      field<ContractEntity, &ContractEntity::endDate>("end_date"),
      field<ContractEntity, &ContractEntity::status>("status"),
      field<ContractEntity, &ContractEntity::description>("description")
    },
    {"id"}};

const TableOperations<EmployeePlotAssignmentEntity> kEmployeePlotAssignmentTable{
    "employee_plot_assignment",
    {
      field<EmployeePlotAssignmentEntity, &EmployeePlotAssignmentEntity::id>("id"),
      field<EmployeePlotAssignmentEntity, &EmployeePlotAssignmentEntity::farmEmployeeId>("farm_employee_id"),
      field<EmployeePlotAssignmentEntity, &EmployeePlotAssignmentEntity::farmPlotId>("farm_plot_id"),
      field<EmployeePlotAssignmentEntity, &EmployeePlotAssignmentEntity::assignmentType>("assignment_type"),
      field<EmployeePlotAssignmentEntity, &EmployeePlotAssignmentEntity::assignedAt>("assigned_at"),
      field<EmployeePlotAssignmentEntity, &EmployeePlotAssignmentEntity::unassignedAt>("unassigned_at"),
      field<EmployeePlotAssignmentEntity, &EmployeePlotAssignmentEntity::notes>("notes")
    },
    {"id"}};

const TableOperations<EmploymentStatusEntity> kEmploymentStatusTable{
    "employment_status",
    {
      field<EmploymentStatusEntity, &EmploymentStatusEntity::id>("id"),
      field<EmploymentStatusEntity, &EmploymentStatusEntity::name>("name")
    },
    {"id"}};

const TableOperations<FarmEntity> kFarmTable{
    "farm",
    {
      field<FarmEntity, &FarmEntity::id>("id"),
      field<FarmEntity, &FarmEntity::name>("name"),
      field<FarmEntity, &FarmEntity::legalName>("legal_name"),
      field<FarmEntity, &FarmEntity::legalAddress>("legal_address"),
      field<FarmEntity, &FarmEntity::actualAddress>("actual_address"),
      field<FarmEntity, &FarmEntity::inn>("inn"),
      field<FarmEntity, &FarmEntity::ogrn>("ogrn"),
      field<FarmEntity, &FarmEntity::status>("status"),
      field<FarmEntity, &FarmEntity::farmType>("farm_type")
    },
    {"id"}};

const TableOperations<FarmAssociationEntity> kFarmAssociationTable{
    "farm_association",
    {
      field<FarmAssociationEntity, &FarmAssociationEntity::id>("id"),
      field<FarmAssociationEntity, &FarmAssociationEntity::inn>("inn"),
      field<FarmAssociationEntity, &FarmAssociationEntity::ogrn>("ogrn"),
      field<FarmAssociationEntity, &FarmAssociationEntity::name>("name"),
      field<FarmAssociationEntity, &FarmAssociationEntity::legalAddress>("legal_address"),
      field<FarmAssociationEntity, &FarmAssociationEntity::status>("status")
    },
    {"id"}};

const TableOperations<FarmEmployeeEntity> kFarmEmployeeTable{
    "farm_employee",
    {
      field<FarmEmployeeEntity, &FarmEmployeeEntity::id>("id"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::personId>("person_id"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::farmId>("farm_id"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::roleId>("role_id"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::employmentStatusId>("employment_status_id"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::hireDate>("hire_date"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::dismissalDate>("dismissal_date"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::salary>("salary"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::employmentContractNumber>("employment_contract_number"),
      field<FarmEmployeeEntity, &FarmEmployeeEntity::isPrimaryWorkplace>("is_primary_workplace")
    },
    {"id"}};

const TableOperations<FarmOwnerEntity> kFarmOwnerTable{
    "farm_owner",
    {
      field<FarmOwnerEntity, &FarmOwnerEntity::id>("id"),
      field<FarmOwnerEntity, &FarmOwnerEntity::personId>("person_id"),
      field<FarmOwnerEntity, &FarmOwnerEntity::status>("status"),
      field<FarmOwnerEntity, &FarmOwnerEntity::rating>("rating")
    },
    {"id"}};

const TableOperations<FarmOwnershipEntity> kFarmOwnershipTable{
    "farm_ownership",
    {
      field<FarmOwnershipEntity, &FarmOwnershipEntity::id>("id"),
      field<FarmOwnershipEntity, &FarmOwnershipEntity::farmId>("farm_id"),
      field<FarmOwnershipEntity, &FarmOwnershipEntity::farmOwnerId>("farm_owner_id"),
      field<FarmOwnershipEntity, &FarmOwnershipEntity::ownershipPercentage>("ownership_percentage"),
      field<FarmOwnershipEntity, &FarmOwnershipEntity::startedAt>("started_at"),
      field<FarmOwnershipEntity, &FarmOwnershipEntity::endedAt>("ended_at")
    },
    {"id"}};

const TableOperations<FarmPlotEntity> kFarmPlotTable{
    "farm_plot",
    {
      field<FarmPlotEntity, &FarmPlotEntity::id>("id"),
      field<FarmPlotEntity, &FarmPlotEntity::farmPlotTypeId>("farm_plot_type_id"),
      field<FarmPlotEntity, &FarmPlotEntity::name>("name"),
      field<FarmPlotEntity, &FarmPlotEntity::area>("area"),
      field<FarmPlotEntity, &FarmPlotEntity::location>("location"),
      field<FarmPlotEntity, &FarmPlotEntity::cadastralNumber>("cadastral_number"),
      field<FarmPlotEntity, &FarmPlotEntity::status>("status")
    },
    {"id"}};

const TableOperations<FarmPlotAssignmentEntity> kFarmPlotAssignmentTable{
    "farm_plot_assignment",
    {
      field<FarmPlotAssignmentEntity, &FarmPlotAssignmentEntity::farmId>("farm_id"),
      field<FarmPlotAssignmentEntity, &FarmPlotAssignmentEntity::farmPlotId>("farm_plot_id"),
      field<FarmPlotAssignmentEntity, &FarmPlotAssignmentEntity::status>("status"),
      field<FarmPlotAssignmentEntity, &FarmPlotAssignmentEntity::notes>("notes")
    },
    {"farm_id", "farm_plot_id"}};

const TableOperations<FarmPlotConsumptionProductEntity> kFarmPlotConsumptionProductTable{
    "farm_plot_consumption_product",
    {
      field<FarmPlotConsumptionProductEntity, &FarmPlotConsumptionProductEntity::productId>("product_id"),
      field<FarmPlotConsumptionProductEntity, &FarmPlotConsumptionProductEntity::farmPlotId>("farm_plot_id"),
      field<FarmPlotConsumptionProductEntity, &FarmPlotConsumptionProductEntity::quantity>("quantity"),
      field<FarmPlotConsumptionProductEntity, &FarmPlotConsumptionProductEntity::consumptionNow>("consumption_now")
    },
    {"product_id", "farm_plot_id"}};

const TableOperations<FarmPlotProductionProductEntity> kFarmPlotProductionProductTable{
    "farm_plot_production_product",
    {
      field<FarmPlotProductionProductEntity, &FarmPlotProductionProductEntity::productId>("product_id"),
      field<FarmPlotProductionProductEntity, &FarmPlotProductionProductEntity::farmPlotId>("farm_plot_id"),
      field<FarmPlotProductionProductEntity, &FarmPlotProductionProductEntity::quantity>("quantity"),
      field<FarmPlotProductionProductEntity, &FarmPlotProductionProductEntity::productionNow>("production_now")
    },
    {"product_id", "farm_plot_id"}};

const TableOperations<FarmPlotTypeEntity> kFarmPlotTypeTable{
    "farm_plot_type",
    {
      field<FarmPlotTypeEntity, &FarmPlotTypeEntity::id>("id"),
      field<FarmPlotTypeEntity, &FarmPlotTypeEntity::name>("name"),
      field<FarmPlotTypeEntity, &FarmPlotTypeEntity::description>("description"),
      field<FarmPlotTypeEntity, &FarmPlotTypeEntity::farmPlotLevel>("farm_plot_level"),
      field<FarmPlotTypeEntity, &FarmPlotTypeEntity::parentId>("parent_id")
    },
    {"id"}};

const TableOperations<FarmRoleEntity> kFarmRoleTable{
    "farm_role",
    {
      field<FarmRoleEntity, &FarmRoleEntity::id>("id"),
      field<FarmRoleEntity, &FarmRoleEntity::name>("name"),
      field<FarmRoleEntity, &FarmRoleEntity::description>("description")
    },
    {"id"}};

const TableOperations<IdentityDocumentTypeEntity> kIdentityDocumentTypeTable{
    "identity_document_type",
    {
      field<IdentityDocumentTypeEntity, &IdentityDocumentTypeEntity::id>("id"),
      field<IdentityDocumentTypeEntity, &IdentityDocumentTypeEntity::code>("code"),
      field<IdentityDocumentTypeEntity, &IdentityDocumentTypeEntity::name>("name"),
      field<IdentityDocumentTypeEntity, &IdentityDocumentTypeEntity::description>("description")
    },
    {"id"}};

const TableOperations<PersonEntity> kPersonTable{
    "person",
    {
      field<PersonEntity, &PersonEntity::id>("id"),
      field<PersonEntity, &PersonEntity::firstName>("first_name"),
      field<PersonEntity, &PersonEntity::lastName>("last_name"),
      field<PersonEntity, &PersonEntity::middleName>("middle_name"),
      field<PersonEntity, &PersonEntity::birthDate>("birth_date"),
      field<PersonEntity, &PersonEntity::phone>("phone"),
      field<PersonEntity, &PersonEntity::email>("email"),
      field<PersonEntity, &PersonEntity::address>("address")
    },
    {"id"}};

const TableOperations<PersonDocumentEntity> kPersonDocumentTable{
    "person_document",
    {
      field<PersonDocumentEntity, &PersonDocumentEntity::id>("id"),
      field<PersonDocumentEntity, &PersonDocumentEntity::personId>("person_id"),
      field<PersonDocumentEntity, &PersonDocumentEntity::documentTypeId>("document_type_id"),
      field<PersonDocumentEntity, &PersonDocumentEntity::documentNumber>("document_number"),
      field<PersonDocumentEntity, &PersonDocumentEntity::issuedBy>("issued_by"),
      field<PersonDocumentEntity, &PersonDocumentEntity::issuedDate>("issued_date"),
      field<PersonDocumentEntity, &PersonDocumentEntity::expirationDate>("expiration_date"),
      field<PersonDocumentEntity, &PersonDocumentEntity::isPrimary>("is_primary")
    },
    {"id"}};

const TableOperations<ProductEntity> kProductTable{
    "product",
    {
      field<ProductEntity, &ProductEntity::id>("id"),
      field<ProductEntity, &ProductEntity::typeId>("type_id"),
      field<ProductEntity, &ProductEntity::name>("name"),
      field<ProductEntity, &ProductEntity::unitId>("unit_id")
    },
    {"id"}};

const TableOperations<ProductTypeEntity> kProductTypeTable{
    "product_type",
    {
      field<ProductTypeEntity, &ProductTypeEntity::id>("id"),
      field<ProductTypeEntity, &ProductTypeEntity::parentId>("parent_id"),
      field<ProductTypeEntity, &ProductTypeEntity::sku>("sku"),
      field<ProductTypeEntity, &ProductTypeEntity::productLevel>("product_level"),
      field<ProductTypeEntity, &ProductTypeEntity::name>("name"),
      field<ProductTypeEntity, &ProductTypeEntity::description>("description")
    },
    {"id"}};

const TableOperations<PurchaseOrderEntity> kPurchaseOrderTable{
    "purchase_order",
    {
      field<PurchaseOrderEntity, &PurchaseOrderEntity::id>("id"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::associationId>("association_id"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::supplierId>("supplier_id"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::deliveryAddress>("delivery_address"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::orderDate>("order_date"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::expectedDeliveryDate>("expected_delivery_date"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::status>("status"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::totalAmount>("total_amount"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::receivedAt>("received_at"),
      field<PurchaseOrderEntity, &PurchaseOrderEntity::createdBy>("created_by")
    },
    {"id"}};

const TableOperations<PurchaseOrderItemEntity> kPurchaseOrderItemTable{
    "purchase_order_item",
    {
      field<PurchaseOrderItemEntity, &PurchaseOrderItemEntity::id>("id"),
      field<PurchaseOrderItemEntity, &PurchaseOrderItemEntity::purchaseOrderId>("purchase_order_id"),
      field<PurchaseOrderItemEntity, &PurchaseOrderItemEntity::productId>("product_id"),
      field<PurchaseOrderItemEntity, &PurchaseOrderItemEntity::quantity>("quantity"),
      field<PurchaseOrderItemEntity, &PurchaseOrderItemEntity::unitPrice>("unit_price"),
      field<PurchaseOrderItemEntity, &PurchaseOrderItemEntity::vatRate>("vat_rate"),
      field<PurchaseOrderItemEntity, &PurchaseOrderItemEntity::currency>("currency")
    },
    {"id"}};

const TableOperations<PurchaseRequisitionEntity> kPurchaseRequisitionTable{
    "purchase_requisition",
    {
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::id>("id"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::farmId>("farm_id"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::productId>("product_id"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::quantity>("quantity"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::maxPricePerUnit>("max_price_per_unit"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::offerDate>("offer_date"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::requiredDate>("required_date"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::priority>("priority"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::validUntil>("valid_until"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::status>("status"),
      field<PurchaseRequisitionEntity, &PurchaseRequisitionEntity::notes>("notes")
    },
    {"id"}};

const TableOperations<SalesRequisitionEntity> kSalesRequisitionTable{
    "sales_requisition",
    {
      field<SalesRequisitionEntity, &SalesRequisitionEntity::id>("id"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::farmId>("farm_id"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::productId>("product_id"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::quantity>("quantity"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::pricePerUnit>("price_per_unit"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::offerDate>("offer_date"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::validUntil>("valid_until"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::status>("status"),
      field<SalesRequisitionEntity, &SalesRequisitionEntity::notes>("notes")
    },
    {"id"}};

const TableOperations<SupplierEntity> kSupplierTable{
    "supplier",
    {
      field<SupplierEntity, &SupplierEntity::id>("id"),
      field<SupplierEntity, &SupplierEntity::name>("name"),
      field<SupplierEntity, &SupplierEntity::legalAddress>("legal_address"),
      field<SupplierEntity, &SupplierEntity::status>("status")
    },
    {"id"}};

const TableOperations<SupplierProductPriceEntity> kSupplierProductPriceTable{
    "supplier_product_price",
    {
      field<SupplierProductPriceEntity, &SupplierProductPriceEntity::id>("id"),
      field<SupplierProductPriceEntity, &SupplierProductPriceEntity::supplierId>("supplier_id"),
      field<SupplierProductPriceEntity, &SupplierProductPriceEntity::productId>("product_id"),
      field<SupplierProductPriceEntity, &SupplierProductPriceEntity::purchasePrice>("purchase_price"),
      field<SupplierProductPriceEntity, &SupplierProductPriceEntity::validFrom>("valid_from"),
      field<SupplierProductPriceEntity, &SupplierProductPriceEntity::validUntil>("valid_until")
    },
    {"id"}};

const TableOperations<UnitEntity> kUnitTable{
    "unit",
    {
      field<UnitEntity, &UnitEntity::id>("id"),
      field<UnitEntity, &UnitEntity::code>("code"),
      field<UnitEntity, &UnitEntity::name>("name")
    },
    {"id"}};

const std::vector<const TableOperationsBase*>& registry() {
  static const std::vector<const TableOperationsBase*> tables{
      &kAssociationEmployeeTable,
      &kAssociationFarmsTable,
      &kAssociationMemberTable,
      &kAssociationRoleTable,
      &kContractTable,
      &kEmployeePlotAssignmentTable,
      &kEmploymentStatusTable,
      &kFarmTable,
      &kFarmAssociationTable,
      &kFarmEmployeeTable,
      &kFarmOwnerTable,
      &kFarmOwnershipTable,
      &kFarmPlotTable,
      &kFarmPlotAssignmentTable,
      &kFarmPlotConsumptionProductTable,
      &kFarmPlotProductionProductTable,
      &kFarmPlotTypeTable,
      &kFarmRoleTable,
      &kIdentityDocumentTypeTable,
      &kPersonTable,
      &kPersonDocumentTable,
      &kProductTable,
      &kProductTypeTable,
      &kPurchaseOrderTable,
      &kPurchaseOrderItemTable,
      &kPurchaseRequisitionTable,
      &kSalesRequisitionTable,
      &kSupplierTable,
      &kSupplierProductPriceTable,
      &kUnitTable
  };
  return tables;
}

const TableOperationsBase& requireTable(std::string_view table) {
  const std::string_view normalized = normalizeTableName(table);
  for (const TableOperationsBase* operations : registry()) {
    if (operations->table() == normalized) {
      return *operations;
    }
  }

  throw std::invalid_argument{"No ODB mapping registered for table: " + std::string{table}};
}

} // namespace

std::vector<SqlRow> selectOdbRows(Database& db,
                                  const std::string& table,
                                  const std::vector<std::string>& columns) {
  return requireTable(table).select(db, columns);
}

std::optional<SqlRow> selectOneOdbRow(Database& db,
                                      const std::string& table,
                                      const std::vector<std::string>& columns,
                                      const std::vector<std::string>& keyColumns,
                                      const std::vector<SqlParameter>& keyValues) {
  return requireTable(table).selectOne(db, columns, keyColumns, keyValues);
}

unsigned long long insertOdbRow(Database& db,
                                const std::string& table,
                                const std::vector<std::string>& columns,
                                const std::vector<SqlParameter>& values) {
  return requireTable(table).insert(db, columns, values);
}

unsigned long long updateOdbRows(Database& db,
                                 const std::string& table,
                                 const std::vector<std::string>& columns,
                                 const std::vector<SqlParameter>& values,
                                 const std::vector<std::string>& keyColumns,
                                 const std::vector<SqlParameter>& keyValues) {
  return requireTable(table).update(db, columns, values, keyColumns, keyValues);
}

unsigned long long deleteOdbRows(Database& db,
                                 const std::string& table,
                                 const std::vector<std::string>& keyColumns,
                                 const std::vector<SqlParameter>& keyValues) {
  return requireTable(table).erase(db, keyColumns, keyValues);
}

} // namespace fasc::server::database
