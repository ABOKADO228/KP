import { Save, X } from "lucide-react";
import type { FormEvent } from "react";

import type { BusinessModule } from "../domain/roles";
import {
  datalistIdForColumn,
  datalistOptionsForColumn,
  inputTypeForColumn,
  isBooleanField,
  isLongTextField,
  labelForColumn,
} from "../domain/resourceFields";

interface ResourceFormProps {
  columns: string[];
  formMode: "create" | "edit";
  isSubmitting: boolean;
  module: BusinessModule;
  onCancel: () => void;
  onChange: (values: Record<string, string>) => void;
  onSubmit: (event: FormEvent<HTMLFormElement>) => void;
  values: Record<string, string>;
}

export function ResourceForm({
  columns,
  formMode,
  isSubmitting,
  module,
  onCancel,
  onChange,
  onSubmit,
  values,
}: ResourceFormProps) {
  return (
    <form className="resource-form" onSubmit={onSubmit}>
      <div className="resource-form-header">
        <div>
          <p className="eyebrow">{module.id}</p>
          <h3>{formMode === "create" ? "Новая запись" : "Изменение записи"}</h3>
        </div>
        <button className="icon-button" onClick={onCancel} title="Закрыть" type="button">
          <X aria-hidden="true" size={18} />
        </button>
      </div>

      <div className="form-grid">
        {columns.map((column) => (
          <FieldInput
            column={column}
            key={column}
            onChange={(value) => onChange({ ...values, [column]: value })}
            value={values[column] ?? ""}
          />
        ))}
      </div>

      <div className="form-footer">
        <button className="primary-button" disabled={isSubmitting} type="submit">
          <Save aria-hidden="true" size={18} />
          {formMode === "create" ? "Создать" : "Сохранить"}
        </button>
        <button className="ghost-button" disabled={isSubmitting} onClick={onCancel} type="button">
          Отмена
        </button>
      </div>
    </form>
  );
}

interface FieldInputProps {
  column: string;
  onChange: (value: string) => void;
  value: string;
}

function FieldInput({ column, onChange, value }: FieldInputProps) {
  const options = datalistOptionsForColumn(column);

  if (isLongTextField(column)) {
    return (
      <label className="field-label">
        <span>{labelForColumn(column)}</span>
        <textarea
          onChange={(event) => onChange(event.target.value)}
          rows={3}
          value={value}
        />
      </label>
    );
  }

  if (isBooleanField(column)) {
    return (
      <label className="field-label">
        <span>{labelForColumn(column)}</span>
        <select onChange={(event) => onChange(event.target.value)} value={value}>
          <option value="">Не задано</option>
          <option value="true">Да</option>
          <option value="false">Нет</option>
        </select>
      </label>
    );
  }

  return (
    <label className="field-label">
      <span>{labelForColumn(column)}</span>
      <input
        list={options.length > 0 ? datalistIdForColumn(column) : undefined}
        onChange={(event) => onChange(event.target.value)}
        step={inputTypeForColumn(column) === "number" ? "any" : undefined}
        type={inputTypeForColumn(column)}
        value={value}
      />
      {options.length > 0 ? (
        <datalist id={datalistIdForColumn(column)}>
          {options.map((option) => (
            <option key={option} value={option} />
          ))}
        </datalist>
      ) : null}
    </label>
  );
}
