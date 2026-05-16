import type { ResourceRow } from "../api/farmApi";
import type { BusinessModule } from "./roles";

export function columnsFor(module: BusinessModule, rows: ResourceRow[]): string[] {
  const rowColumns = rows.flatMap((row) => Object.keys(row));
  const uniqueColumns = new Set([...module.defaultColumns, ...rowColumns]);
  return Array.from(uniqueColumns).slice(0, 10);
}

export function formColumnsFor(
  module: BusinessModule,
  rows: ResourceRow[],
  mode: "create" | "edit",
): string[] {
  const rowColumns = rows.flatMap((row) => Object.keys(row));
  const allColumns = Array.from(
    new Set([...(module.formColumns ?? module.defaultColumns), ...rowColumns]),
  );
  const keyColumns = keyColumnsFor(module);

  return allColumns.filter((column) => {
    if (mode === "edit") {
      return !keyColumns.includes(column);
    }
    return !isGeneratedSimpleId(module, column);
  });
}

export function createEmptyFormValues(
  module: BusinessModule,
  rows: ResourceRow[],
): Record<string, string> {
  return Object.fromEntries(
    formColumnsFor(module, rows, "create").map((column) => [column, ""]),
  );
}

export function createFormValuesFromRow(
  module: BusinessModule,
  rows: ResourceRow[],
  row: ResourceRow,
): Record<string, string> {
  return Object.fromEntries(
    formColumnsFor(module, rows, "edit").map((column) => [
      column,
      valueToFormString(row[column]),
    ]),
  );
}

export function payloadFromForm(
  module: BusinessModule,
  rows: ResourceRow[],
  values: Record<string, string>,
  mode: "create" | "edit",
): ResourceRow {
  const payload: ResourceRow = {};
  for (const column of formColumnsFor(module, rows, mode)) {
    const rawValue = values[column]?.trim() ?? "";
    if (!rawValue) {
      continue;
    }
    payload[column] = coerceFieldValue(column, rawValue);
  }

  if (Object.keys(payload).length === 0) {
    throw new Error("Заполните хотя бы одно поле");
  }

  return payload;
}

export function keyFromRow(module: BusinessModule, row: ResourceRow): ResourceRow {
  const key: ResourceRow = {};
  for (const column of keyColumnsFor(module)) {
    const value = row[column];
    if (value === null || value === undefined || value === "") {
      throw new Error(`В строке нет ключевого поля ${column}`);
    }
    key[column] = value;
  }
  return key;
}

export function keyColumnsFor(module: BusinessModule): string[] {
  return [...(module.keyColumns ?? ["id"])];
}

export function describeKey(key: ResourceRow): string {
  return Object.entries(key)
    .map(([column, value]) => `${column}=${String(value)}`)
    .join(", ");
}

export function inputTypeForColumn(column: string): "date" | "number" | "text" {
  if (isDateField(column)) {
    return "date";
  }
  if (isNumericField(column)) {
    return "number";
  }
  return "text";
}

export function isBooleanField(column: string): boolean {
  return column.startsWith("is_") || column.startsWith("has_");
}

export function isLongTextField(column: string): boolean {
  return (
    column.includes("address") ||
    column.includes("description") ||
    column.includes("notes")
  );
}

export function datalistIdForColumn(column: string): string {
  return `options-${column}`;
}

export function datalistOptionsForColumn(column: string): string[] {
  if (column === "status") {
    return [
      "active",
      "inactive",
      "pending",
      "approved",
      "rejected",
      "completed",
      "cancelled",
      "suspended",
    ];
  }
  if (column === "farm_type") {
    return ["peasant_farm", "individual_entrepreneur", "legal_entity"];
  }
  if (column === "currency") {
    return ["RUB", "USD", "EUR"];
  }
  if (column.endsWith("_type") || column === "assignment_type") {
    return ["primary", "secondary", "temporary"];
  }
  return [];
}

export function labelForColumn(column: string): string {
  return column
    .split("_")
    .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
    .join(" ");
}

export function rowKey(row: ResourceRow, index: number): string {
  const id = row.id ?? row.farm_id ?? row.product_id ?? row.association_id;
  return `${String(id ?? "row")}-${index}`;
}

export function formatValue(value: unknown): string {
  if (value === null || value === undefined) {
    return "—";
  }
  if (typeof value === "boolean") {
    return value ? "Да" : "Нет";
  }
  if (typeof value === "object") {
    return JSON.stringify(value);
  }
  return String(value);
}

function isGeneratedSimpleId(module: BusinessModule, column: string): boolean {
  const keyColumns = keyColumnsFor(module);
  return column === "id" && keyColumns.length === 1 && keyColumns[0] === "id";
}

function valueToFormString(value: unknown): string {
  if (value === null || value === undefined) {
    return "";
  }
  return String(value);
}

function coerceFieldValue(column: string, value: string): string | number | boolean {
  if (isBooleanField(column)) {
    return value === "true";
  }
  if (isNumericField(column)) {
    const numericValue = Number(value);
    if (!Number.isFinite(numericValue)) {
      throw new Error(`Поле ${column} должно быть числом`);
    }
    return numericValue;
  }
  return value;
}

function isNumericField(column: string): boolean {
  return (
    column === "id" ||
    column.endsWith("_id") ||
    column.endsWith("_level") ||
    [
      "area",
      "created_by",
      "consumption_now",
      "farm_plot_level",
      "max_price_per_unit",
      "ownership_percentage",
      "price_per_unit",
      "priority",
      "product_level",
      "production_now",
      "purchase_price",
      "quantity",
      "salary",
      "total_amount",
      "unit_price",
      "vat_rate",
    ].includes(column)
  );
}

function isDateField(column: string): boolean {
  return (
    column.endsWith("_date") ||
    column.endsWith("_at") ||
    column.endsWith("_from") ||
    column.endsWith("_until") ||
    ["start_date", "end_date", "sign_date", "valid_from", "valid_until"].includes(column)
  );
}
