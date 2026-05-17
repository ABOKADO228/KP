import type { ResourceRow } from "../api/farmApi";

export type ResourceFilterOperator =
  | "contains"
  | "equals"
  | "not_equals"
  | "empty"
  | "not_empty";

export interface ResourceFilter {
  column: string;
  id: string;
  operator: ResourceFilterOperator;
  value: string;
}

export const resourceFilterOperators: {
  id: ResourceFilterOperator;
  title: string;
}[] = [
  { id: "contains", title: "Содержит" },
  { id: "equals", title: "Равно" },
  { id: "not_equals", title: "Не равно" },
  { id: "empty", title: "Пусто" },
  { id: "not_empty", title: "Заполнено" },
];

export function filterResourceRows(
  rows: ResourceRow[],
  columns: string[],
  searchQuery: string,
  filters: ResourceFilter[],
): ResourceRow[] {
  const normalizedQuery = normalize(searchQuery);
  const searchableColumns = columns.length > 0 ? columns : columnsFromRows(rows);

  return rows.filter((row) => {
    if (normalizedQuery && !rowMatchesSearch(row, searchableColumns, normalizedQuery)) {
      return false;
    }

    return filters.every((filter) => rowMatchesFilter(row, filter));
  });
}

export function filterRequiresValue(operator: ResourceFilterOperator): boolean {
  return operator !== "empty" && operator !== "not_empty";
}

export function titleForFilterOperator(operator: ResourceFilterOperator): string {
  return resourceFilterOperators.find((candidate) => candidate.id === operator)?.title ?? operator;
}

function rowMatchesSearch(
  row: ResourceRow,
  columns: string[],
  normalizedQuery: string,
): boolean {
  return columns.some((column) => normalize(valueToText(row[column])).includes(normalizedQuery));
}

function rowMatchesFilter(row: ResourceRow, filter: ResourceFilter): boolean {
  const rawValue = row[filter.column];
  const text = valueToText(rawValue);
  const normalizedValue = normalize(text);
  const normalizedFilterValue = normalize(filter.value);

  switch (filter.operator) {
    case "contains":
      return normalizedValue.includes(normalizedFilterValue);
    case "equals":
      return normalizedValue === normalizedFilterValue;
    case "not_equals":
      return normalizedValue !== normalizedFilterValue;
    case "empty":
      return isEmptyValue(rawValue);
    case "not_empty":
      return !isEmptyValue(rawValue);
  }
}

function columnsFromRows(rows: ResourceRow[]): string[] {
  return Array.from(new Set(rows.flatMap((row) => Object.keys(row))));
}

function isEmptyValue(value: unknown): boolean {
  return value === null || value === undefined || value === "";
}

function valueToText(value: unknown): string {
  if (value === null || value === undefined) {
    return "";
  }
  if (typeof value === "object") {
    return JSON.stringify(value);
  }
  return String(value);
}

function normalize(value: string): string {
  return value.trim().toLocaleLowerCase("ru-RU");
}
