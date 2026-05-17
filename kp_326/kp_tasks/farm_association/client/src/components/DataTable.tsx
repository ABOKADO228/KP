import { Pencil, Trash2 } from "lucide-react";

import type { ResourceRow } from "../api/farmApi";
import { formatValue, rowKey } from "../domain/resourceFields";

interface DataTableProps {
  columns: string[];
  emptyLabel?: string;
  isLoading: boolean;
  onDelete: (row: ResourceRow) => void;
  onEdit: (row: ResourceRow) => void;
  rows: ResourceRow[];
  writable: boolean;
}

export function DataTable({
  columns,
  emptyLabel = "Нет данных",
  isLoading,
  onDelete,
  onEdit,
  rows,
  writable,
}: DataTableProps) {
  if (isLoading) {
    return <div className="empty-state">Загрузка</div>;
  }

  if (rows.length === 0) {
    return <div className="empty-state">{emptyLabel}</div>;
  }

  return (
    <div className="table-scroll">
      <table>
        <thead>
          <tr>
            {columns.map((column) => (
              <th key={column}>{column}</th>
            ))}
            {writable ? <th>Действия</th> : null}
          </tr>
        </thead>
        <tbody>
          {rows.map((row, index) => (
            <tr key={rowKey(row, index)}>
              {columns.map((column) => (
                <td key={column}>{formatValue(row[column])}</td>
              ))}
              {writable ? (
                <td>
                  <div className="row-actions">
                    <button
                      className="icon-button compact"
                      onClick={() => onEdit(row)}
                      title="Изменить"
                      type="button"
                    >
                      <Pencil aria-hidden="true" size={16} />
                    </button>
                    <button
                      className="icon-button compact danger"
                      onClick={() => onDelete(row)}
                      title="Удалить"
                      type="button"
                    >
                      <Trash2 aria-hidden="true" size={16} />
                    </button>
                  </div>
                </td>
              ) : null}
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
