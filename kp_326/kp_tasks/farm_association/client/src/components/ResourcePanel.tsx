import { Plus, RefreshCw, Search, SlidersHorizontal, X } from "lucide-react";
import { FormEvent, useCallback, useEffect, useMemo, useState } from "react";

import type { FarmApiClient, ResourceRow } from "../api/farmApi";
import {
  allColumnsFor,
  columnsFor,
  createEmptyFormValues,
  createFormValuesFromRow,
  describeKey,
  formColumnsFor,
  keyFromRow,
  payloadFromForm,
} from "../domain/resourceFields";
import {
  filterRequiresValue,
  filterResourceRows,
  resourceFilterOperators,
  titleForFilterOperator,
  type ResourceFilter,
  type ResourceFilterOperator,
} from "../domain/resourceFilters";
import { canMutateModule, type BusinessModule, type UserRole } from "../domain/roles";
import { errorMessage } from "../utils/errors";
import { DataTable } from "./DataTable";
import { DeleteConfirmation } from "./DeleteConfirmation";
import { ResourceForm } from "./ResourceForm";

interface ResourcePanelProps {
  api: FarmApiClient;
  module: BusinessModule;
  role: UserRole;
}

export function ResourcePanel({ api, module, role }: ResourcePanelProps) {
  const [rows, setRows] = useState<ResourceRow[]>([]);
  const [resourceName, setResourceName] = useState(module.id);
  const [isLoading, setIsLoading] = useState(false);
  const [isMutating, setIsMutating] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [notice, setNotice] = useState<string | null>(null);
  const [formMode, setFormMode] = useState<"create" | "edit" | null>(null);
  const [editingRow, setEditingRow] = useState<ResourceRow | null>(null);
  const [pendingDeleteRow, setPendingDeleteRow] = useState<ResourceRow | null>(null);
  const [formValues, setFormValues] = useState<Record<string, string>>({});
  const [searchQuery, setSearchQuery] = useState("");
  const [filters, setFilters] = useState<ResourceFilter[]>([]);
  const [filterColumn, setFilterColumn] = useState("");
  const [filterOperator, setFilterOperator] =
    useState<ResourceFilterOperator>("contains");
  const [filterValue, setFilterValue] = useState("");

  const loadRows = useCallback(async () => {
    setIsLoading(true);
    setError(null);

    try {
      const response = await api.listResource(module.endpoint);
      setRows(response.rows);
      setResourceName(response.resource);
    } catch (caught) {
      setRows([]);
      setError(errorMessage(caught));
    } finally {
      setIsLoading(false);
    }
  }, [api, module.endpoint]);

  useEffect(() => {
    void loadRows();
  }, [loadRows]);

  useEffect(() => {
    setFormMode(null);
    setEditingRow(null);
    setPendingDeleteRow(null);
    setFormValues({});
    setNotice(null);
    setSearchQuery("");
    setFilters([]);
    setFilterColumn("");
    setFilterOperator("contains");
    setFilterValue("");
  }, [module.id]);

  const writable = canMutateModule(role, module);
  const tableColumns = columnsFor(module, rows);
  const filterColumns = useMemo(() => allColumnsFor(module, rows), [module, rows]);
  const filteredRows = useMemo(
    () => filterResourceRows(rows, filterColumns, searchQuery, filters),
    [filterColumns, filters, rows, searchQuery],
  );
  const isFilterValueRequired = filterRequiresValue(filterOperator);

  useEffect(() => {
    if (filterColumns.length === 0) {
      setFilterColumn("");
      return;
    }

    setFilterColumn((current) =>
      current && filterColumns.includes(current) ? current : filterColumns[0],
    );
  }, [filterColumns]);

  function openCreateForm() {
    setError(null);
    setNotice(null);
    setEditingRow(null);
    setPendingDeleteRow(null);
    setFormMode("create");
    setFormValues(createEmptyFormValues(module, rows));
  }

  function openEditForm(row: ResourceRow) {
    setError(null);
    setNotice(null);
    setEditingRow(row);
    setPendingDeleteRow(null);
    setFormMode("edit");
    setFormValues(createFormValuesFromRow(module, rows, row));
  }

  function closeForm() {
    setFormMode(null);
    setEditingRow(null);
    setPendingDeleteRow(null);
    setFormValues({});
  }

  function addFilter(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    const value = filterValue.trim();

    if (!filterColumn || (isFilterValueRequired && !value)) {
      return;
    }

    setFilters((current) => [
      ...current,
      {
        column: filterColumn,
        id: crypto.randomUUID(),
        operator: filterOperator,
        value,
      },
    ]);
    setFilterValue("");
  }

  function removeFilter(id: string) {
    setFilters((current) => current.filter((filter) => filter.id !== id));
  }

  function clearTableConditions() {
    setSearchQuery("");
    setFilters([]);
    setFilterValue("");
  }

  async function submitForm(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    if (!formMode) {
      return;
    }

    setIsMutating(true);
    setError(null);
    setNotice(null);

    try {
      if (formMode === "create") {
        await api.createResource(
          module.endpoint,
          payloadFromForm(module, rows, formValues, "create"),
        );
        setNotice("Запись создана");
      } else {
        if (!editingRow) {
          throw new Error("Не выбрана запись для изменения");
        }
        await api.updateResource(
          module.endpoint,
          keyFromRow(module, editingRow),
          payloadFromForm(module, rows, formValues, "edit"),
        );
        setNotice("Запись изменена");
      }

      closeForm();
      await loadRows();
    } catch (caught) {
      setError(errorMessage(caught));
    } finally {
      setIsMutating(false);
    }
  }

  function requestDelete(row: ResourceRow) {
    setError(null);
    setNotice(null);
    setFormMode(null);
    setEditingRow(null);
    setPendingDeleteRow(row);
  }

  async function confirmDelete() {
    if (!pendingDeleteRow) {
      return;
    }

    setIsMutating(true);
    setError(null);
    setNotice(null);

    try {
      const key = keyFromRow(module, pendingDeleteRow);
      await api.deleteResource(module.endpoint, key);
      setNotice("Запись удалена");
      closeForm();
      await loadRows();
    } catch (caught) {
      setError(errorMessage(caught));
    } finally {
      setIsMutating(false);
    }
  }

  return (
    <section className="data-panel" aria-labelledby="module-title">
      <div className="panel-header">
        <div>
          <p className="eyebrow">{resourceName}</p>
          <h2 id="module-title">{module.title}</h2>
          <p className="process-text">{module.process}</p>
        </div>
        <div className="panel-actions">
          <span className={writable ? "access-badge write" : "access-badge read"}>
            {writable ? "Запись" : "Просмотр"}
          </span>
          {writable ? (
            <button
              className="primary-button"
              disabled={isMutating}
              onClick={openCreateForm}
              type="button"
            >
              <Plus aria-hidden="true" size={18} />
              Создать
            </button>
          ) : null}
          <button
            className="icon-button"
            disabled={isLoading || isMutating}
            onClick={() => void loadRows()}
            title="Обновить"
            type="button"
          >
            <RefreshCw aria-hidden="true" size={18} />
          </button>
        </div>
      </div>

      {error ? <div className="error-banner">{error}</div> : null}
      {notice ? <div className="notice-banner">{notice}</div> : null}

      {formMode ? (
        <ResourceForm
          columns={formColumnsFor(module, rows, formMode)}
          formMode={formMode}
          isSubmitting={isMutating}
          module={module}
          onCancel={closeForm}
          onChange={setFormValues}
          onSubmit={submitForm}
          values={formValues}
        />
      ) : null}

      {rows.length > 0 || searchQuery || filters.length > 0 ? (
        <div className="table-tools" aria-label="Поиск и фильтрация записей">
          <label className="field-label table-search">
            <span>Поиск</span>
            <span className="input-with-icon">
              <Search aria-hidden="true" size={18} />
              <input
                onChange={(event) => setSearchQuery(event.target.value)}
                placeholder="Любое значение"
                type="search"
                value={searchQuery}
              />
            </span>
          </label>

          <form className="filter-builder" onSubmit={addFilter}>
            <label className="field-label">
              <span>Колонка</span>
              <select
                disabled={filterColumns.length === 0}
                onChange={(event) => setFilterColumn(event.target.value)}
                value={filterColumn}
              >
                {filterColumns.map((column) => (
                  <option key={column} value={column}>
                    {column}
                  </option>
                ))}
              </select>
            </label>

            <label className="field-label">
              <span>Условие</span>
              <select
                onChange={(event) => {
                  const nextOperator = event.target.value as ResourceFilterOperator;
                  setFilterOperator(nextOperator);
                  if (!filterRequiresValue(nextOperator)) {
                    setFilterValue("");
                  }
                }}
                value={filterOperator}
              >
                {resourceFilterOperators.map((operator) => (
                  <option key={operator.id} value={operator.id}>
                    {operator.title}
                  </option>
                ))}
              </select>
            </label>

            <label className="field-label">
              <span>Значение</span>
              <input
                disabled={!isFilterValueRequired}
                onChange={(event) => setFilterValue(event.target.value)}
                placeholder={isFilterValueRequired ? "Значение" : "—"}
                value={filterValue}
              />
            </label>

            <div className="filter-actions">
              <button
                className="icon-button text-button"
                disabled={!filterColumn || (isFilterValueRequired && !filterValue.trim())}
                type="submit"
              >
                <SlidersHorizontal aria-hidden="true" size={18} />
                Добавить
              </button>
              {searchQuery || filters.length > 0 ? (
                <button
                  className="ghost-button"
                  onClick={clearTableConditions}
                  type="button"
                >
                  Сбросить
                </button>
              ) : null}
            </div>
          </form>

          <div className="table-results">
            <span>
              {filteredRows.length} / {rows.length}
            </span>
            {filters.length > 0 ? (
              <div className="filter-list">
                {filters.map((filter) => (
                  <span className="filter-chip" key={filter.id}>
                    {filter.column} {titleForFilterOperator(filter.operator)}
                    {filterRequiresValue(filter.operator) ? ` ${filter.value}` : ""}
                    <button
                      className="chip-remove"
                      onClick={() => removeFilter(filter.id)}
                      title="Убрать фильтр"
                      type="button"
                    >
                      <X aria-hidden="true" size={14} />
                    </button>
                  </span>
                ))}
              </div>
            ) : null}
          </div>
        </div>
      ) : null}

      <DataTable
        columns={tableColumns}
        emptyLabel={rows.length === 0 ? "Нет данных" : "Нет записей по условиям"}
        isLoading={isLoading}
        onDelete={requestDelete}
        onEdit={openEditForm}
        rows={filteredRows}
        writable={writable}
      />

      {pendingDeleteRow ? (
        <DeleteConfirmation
          isSubmitting={isMutating}
          keyDescription={describeKey(keyFromRow(module, pendingDeleteRow))}
          moduleTitle={module.title}
          onCancel={() => setPendingDeleteRow(null)}
          onConfirm={() => void confirmDelete()}
        />
      ) : null}
    </section>
  );
}
