import { Trash2 } from "lucide-react";

interface DeleteConfirmationProps {
  isSubmitting: boolean;
  keyDescription: string;
  moduleTitle: string;
  onCancel: () => void;
  onConfirm: () => void;
}

export function DeleteConfirmation({
  isSubmitting,
  keyDescription,
  moduleTitle,
  onCancel,
  onConfirm,
}: DeleteConfirmationProps) {
  return (
    <section className="delete-confirmation" aria-label="Подтверждение удаления">
      <div>
        <p className="eyebrow">{moduleTitle}</p>
        <h3>Удалить запись?</h3>
        <p>{keyDescription}</p>
      </div>
      <div className="form-footer">
        <button
          className="primary-button danger"
          disabled={isSubmitting}
          onClick={onConfirm}
          type="button"
        >
          <Trash2 aria-hidden="true" size={18} />
          Удалить
        </button>
        <button className="ghost-button" disabled={isSubmitting} onClick={onCancel} type="button">
          Отмена
        </button>
      </div>
    </section>
  );
}
