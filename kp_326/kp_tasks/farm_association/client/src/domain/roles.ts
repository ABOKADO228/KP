export type UserRole =
  | "agriculture_admin"
  | "association_director"
  | "farm_owner"
  | "agronomist"
  | "procurement_manager"
  | "sales_manager"
  | "farm_worker";

export type AccessLevel = "read" | "write" | "approve" | "admin";

export type BusinessArea =
  | "associations"
  | "farms"
  | "plots"
  | "products"
  | "procurement"
  | "sales"
  | "suppliers";

export type ModuleIcon =
  | "association"
  | "contract"
  | "farm"
  | "field"
  | "member"
  | "order"
  | "product"
  | "supplier";

export interface RoleDefinition {
  id: UserRole;
  title: string;
  access: AccessLevel;
}

export interface BusinessModule {
  id: string;
  title: string;
  endpoint: `/api/${string}`;
  area: BusinessArea;
  icon: ModuleIcon;
  process: string;
  defaultColumns: string[];
  visibleBy: readonly UserRole[];
  writableBy: readonly UserRole[];
}

export const roleDefinitions: readonly RoleDefinition[] = [
  {
    id: "agriculture_admin",
    title: "Администратор сельского хозяйства",
    access: "admin",
  },
  {
    id: "association_director",
    title: "Руководитель ассоциации",
    access: "approve",
  },
  {
    id: "farm_owner",
    title: "Владелец хозяйства",
    access: "write",
  },
  {
    id: "agronomist",
    title: "Агроном",
    access: "write",
  },
  {
    id: "procurement_manager",
    title: "Менеджер закупок",
    access: "write",
  },
  {
    id: "sales_manager",
    title: "Менеджер продаж",
    access: "write",
  },
  {
    id: "farm_worker",
    title: "Сотрудник хозяйства",
    access: "read",
  },
];

const associationRoles: readonly UserRole[] = [
  "association_director",
  "procurement_manager",
  "sales_manager",
];

const farmRoles: readonly UserRole[] = [
  "association_director",
  "farm_owner",
  "agronomist",
  "farm_worker",
];

const procurementRoles: readonly UserRole[] = [
  "association_director",
  "procurement_manager",
];

const productRoles: readonly UserRole[] = [
  "association_director",
  "farm_owner",
  "agronomist",
  "procurement_manager",
  "sales_manager",
  "farm_worker",
];

export const businessModules: readonly BusinessModule[] = [
  {
    id: "farm_association",
    title: "Ассоциации",
    endpoint: "/api/farm_association",
    area: "associations",
    icon: "association",
    process: "Управление фермерскими ассоциациями",
    defaultColumns: ["id", "name", "inn", "ogrn", "status"],
    visibleBy: associationRoles,
    writableBy: ["association_director"],
  },
  {
    id: "association_member",
    title: "Участники",
    endpoint: "/api/association_member",
    area: "associations",
    icon: "member",
    process: "Учет членства в ассоциации",
    defaultColumns: ["id", "association_id", "person_id", "joined_at", "status"],
    visibleBy: associationRoles,
    writableBy: ["association_director"],
  },
  {
    id: "association_employee",
    title: "Сотрудники ассоциации",
    endpoint: "/api/association_employee",
    area: "associations",
    icon: "member",
    process: "Назначение сотрудников ассоциации",
    defaultColumns: ["id", "association_id", "person_id", "role_id", "status"],
    visibleBy: associationRoles,
    writableBy: ["association_director"],
  },
  {
    id: "association_farms",
    title: "Хозяйства ассоциации",
    endpoint: "/api/association_farms",
    area: "associations",
    icon: "farm",
    process: "Привязка хозяйств к ассоциациям",
    defaultColumns: ["farm_id", "association_id", "joined_at", "status"],
    visibleBy: associationRoles,
    writableBy: ["association_director"],
  },
  {
    id: "farm",
    title: "Хозяйства",
    endpoint: "/api/farm",
    area: "farms",
    icon: "farm",
    process: "Ведение карточек фермерских хозяйств",
    defaultColumns: ["id", "name", "legal_name", "inn", "status", "farm_type"],
    visibleBy: farmRoles,
    writableBy: ["association_director", "farm_owner"],
  },
  {
    id: "farm_owner",
    title: "Владельцы",
    endpoint: "/api/farm_owner",
    area: "farms",
    icon: "member",
    process: "Учет владельцев хозяйств",
    defaultColumns: ["id", "person_id", "status"],
    visibleBy: ["association_director", "farm_owner"],
    writableBy: ["association_director", "farm_owner"],
  },
  {
    id: "farm_employee",
    title: "Сотрудники хозяйств",
    endpoint: "/api/farm_employee",
    area: "farms",
    icon: "member",
    process: "Сотрудники, роли и статусы занятости",
    defaultColumns: ["id", "farm_id", "person_id", "role_id", "status_id"],
    visibleBy: ["association_director", "farm_owner"],
    writableBy: ["association_director", "farm_owner"],
  },
  {
    id: "farm_plot",
    title: "Участки",
    endpoint: "/api/farm_plot",
    area: "plots",
    icon: "field",
    process: "Учет земельных участков",
    defaultColumns: ["id", "name", "area", "location", "status"],
    visibleBy: farmRoles,
    writableBy: ["farm_owner", "agronomist"],
  },
  {
    id: "farm_plot_assignment",
    title: "Назначения участков",
    endpoint: "/api/farm_plot_assignment",
    area: "plots",
    icon: "field",
    process: "Закрепление участков за хозяйствами",
    defaultColumns: ["farm_id", "farm_plot_id", "status", "notes"],
    visibleBy: farmRoles,
    writableBy: ["farm_owner", "agronomist"],
  },
  {
    id: "employee_plot_assignment",
    title: "Назначения сотрудников",
    endpoint: "/api/employee_plot_assignment",
    area: "plots",
    icon: "field",
    process: "Закрепление сотрудников за участками",
    defaultColumns: ["id", "farm_employee_id", "farm_plot_id", "started_at", "status"],
    visibleBy: farmRoles,
    writableBy: ["farm_owner", "agronomist"],
  },
  {
    id: "farm_plot_production_product",
    title: "Производство",
    endpoint: "/api/farm_plot_production_product",
    area: "plots",
    icon: "product",
    process: "Выход продукции на участках",
    defaultColumns: ["product_id", "farm_plot_id", "quantity", "production_now"],
    visibleBy: ["farm_owner", "agronomist", "farm_worker"],
    writableBy: ["agronomist", "farm_worker"],
  },
  {
    id: "farm_plot_consumption_product",
    title: "Потребление",
    endpoint: "/api/farm_plot_consumption_product",
    area: "plots",
    icon: "product",
    process: "Расход ресурсов на участках",
    defaultColumns: ["product_id", "farm_plot_id", "quantity", "consumption_now"],
    visibleBy: ["farm_owner", "agronomist", "farm_worker"],
    writableBy: ["agronomist", "farm_worker"],
  },
  {
    id: "product",
    title: "Продукция",
    endpoint: "/api/product",
    area: "products",
    icon: "product",
    process: "Каталог продукции",
    defaultColumns: ["id", "product_type_id", "name", "unit_id", "status"],
    visibleBy: productRoles,
    writableBy: ["association_director", "procurement_manager"],
  },
  {
    id: "supplier",
    title: "Поставщики",
    endpoint: "/api/supplier",
    area: "suppliers",
    icon: "supplier",
    process: "Реестр поставщиков",
    defaultColumns: ["id", "name", "inn", "ogrn", "status"],
    visibleBy: procurementRoles,
    writableBy: ["procurement_manager"],
  },
  {
    id: "supplier_product_price",
    title: "Цены поставщиков",
    endpoint: "/api/supplier_product_price",
    area: "suppliers",
    icon: "supplier",
    process: "Цены поставщиков на продукцию",
    defaultColumns: ["id", "supplier_id", "product_id", "price", "valid_from"],
    visibleBy: procurementRoles,
    writableBy: ["procurement_manager"],
  },
  {
    id: "contract",
    title: "Договоры",
    endpoint: "/api/contract",
    area: "suppliers",
    icon: "contract",
    process: "Договоры с хозяйствами и поставщиками",
    defaultColumns: ["id", "association_id", "farm_id", "supplier_id", "status"],
    visibleBy: [
      "association_director",
      "farm_owner",
      "procurement_manager",
      "sales_manager",
    ],
    writableBy: ["association_director", "procurement_manager"],
  },
  {
    id: "purchase_requisition",
    title: "Заявки на закупку",
    endpoint: "/api/purchase_requisition",
    area: "procurement",
    icon: "order",
    process: "Потребности хозяйств в закупках",
    defaultColumns: ["id", "farm_id", "product_id", "quantity", "required_date", "status"],
    visibleBy: ["association_director", "farm_owner", "procurement_manager"],
    writableBy: ["farm_owner", "procurement_manager"],
  },
  {
    id: "purchase_order",
    title: "Заказы",
    endpoint: "/api/purchase_order",
    area: "procurement",
    icon: "order",
    process: "Заказы поставщикам",
    defaultColumns: ["id", "association_id", "supplier_id", "order_date", "status", "total_amount"],
    visibleBy: procurementRoles,
    writableBy: ["procurement_manager"],
  },
  {
    id: "purchase_order_item",
    title: "Позиции заказов",
    endpoint: "/api/purchase_order_item",
    area: "procurement",
    icon: "order",
    process: "Состав заказов поставщикам",
    defaultColumns: ["id", "purchase_order_id", "product_id", "quantity", "price_per_unit"],
    visibleBy: procurementRoles,
    writableBy: ["procurement_manager"],
  },
  {
    id: "sales_requisition",
    title: "Заявки на продажу",
    endpoint: "/api/sales_requisition",
    area: "sales",
    icon: "order",
    process: "Предложения хозяйств на продажу продукции",
    defaultColumns: ["id", "farm_id", "product_id", "quantity", "price_per_unit", "status"],
    visibleBy: ["association_director", "farm_owner", "sales_manager"],
    writableBy: ["farm_owner", "sales_manager"],
  },
];

export function getRoleTitle(role: UserRole): string {
  return roleDefinitions.find((definition) => definition.id === role)?.title ?? role;
}

export function isKnownRole(role: string): role is UserRole {
  return roleDefinitions.some((definition) => definition.id === role);
}

export function canAccessModule(role: UserRole, module: BusinessModule): boolean {
  return role === "agriculture_admin" || module.visibleBy.includes(role);
}

export function canMutateModule(role: UserRole, module: BusinessModule): boolean {
  return role === "agriculture_admin" || module.writableBy.includes(role);
}

export function getAccessibleModules(role: UserRole): readonly BusinessModule[] {
  return businessModules.filter((module) => canAccessModule(role, module));
}
