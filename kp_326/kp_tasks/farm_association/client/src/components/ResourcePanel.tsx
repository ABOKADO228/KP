import { Plus, RefreshCw } from "lucide-react";
import { FormEvent, useCallback, useEffect, useState } from "react";

import type { FarmApiClient, ResourceRow } from "../api/farmApi";
import {
  columnsFor,
  createEmptyFormValues,
  createFormValuesFromRow,
  describeKey,
  formColumnsFor,
  keyFromRow,
  payloadFromForm,
} from "../domain/resourceFields";
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
  }, [module.id]);

  const writable = canMutateModule(role, module);
  const tableColumns = columnsFor(module, rows);

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

      <DataTable
        columns={tableColumns}
        isLoading={isLoading}
        onDelete={requestDelete}
        onEdit={openEditForm}
        rows={rows}
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
