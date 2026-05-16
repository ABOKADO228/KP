import { FormEvent, useState } from "react";
import { UserPlus } from "lucide-react";

import type { FarmApiClient } from "../api/farmApi";
import { getRoleTitle, roleDefinitions, type UserRole } from "../domain/roles";
import { errorMessage } from "../utils/errors";

interface UserAdministrationPanelProps {
  api: FarmApiClient;
}

export function UserAdministrationPanel({ api }: UserAdministrationPanelProps) {
  const [login, setLogin] = useState("");
  const [password, setPassword] = useState("");
  const [role, setRole] = useState<UserRole>("farm_worker");
  const [error, setError] = useState<string | null>(null);
  const [notice, setNotice] = useState<string | null>(null);
  const [isSubmitting, setIsSubmitting] = useState(false);

  async function submitUser(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setError(null);
    setNotice(null);
    setIsSubmitting(true);

    try {
      const created = await api.createUser({ login, password, role });
      setNotice(`${created.login}: ${getRoleTitle(created.role)}`);
      setLogin("");
      setPassword("");
      setRole("farm_worker");
    } catch (caught) {
      setError(errorMessage(caught));
    } finally {
      setIsSubmitting(false);
    }
  }

  return (
    <section className="data-panel" aria-labelledby="users-title">
      <div className="panel-header">
        <div>
          <p className="eyebrow">users</p>
          <h2 id="users-title">Пользователи</h2>
        </div>
        <span className="access-badge write">Администрирование</span>
      </div>

      {error ? <div className="error-banner">{error}</div> : null}
      {notice ? <div className="notice-banner">{notice}</div> : null}

      <form className="resource-form" onSubmit={submitUser}>
        <div className="resource-form-header">
          <h3>Новый пользователь</h3>
        </div>

        <div className="form-grid">
          <label className="field-label">
            <span>Логин</span>
            <input
              autoComplete="username"
              name="login"
              onChange={(event) => setLogin(event.target.value)}
              required
              value={login}
            />
          </label>

          <label className="field-label">
            <span>Пароль</span>
            <input
              autoComplete="new-password"
              minLength={8}
              name="password"
              onChange={(event) => setPassword(event.target.value)}
              required
              type="password"
              value={password}
            />
          </label>

          <label className="field-label">
            <span>Роль</span>
            <select
              name="role"
              onChange={(event) => setRole(event.target.value as UserRole)}
              value={role}
            >
              {roleDefinitions.map((definition) => (
                <option key={definition.id} value={definition.id}>
                  {definition.title}
                </option>
              ))}
            </select>
          </label>
        </div>

        <div className="form-footer">
          <button className="primary-button" disabled={isSubmitting} type="submit">
            <UserPlus aria-hidden="true" size={18} />
            Создать
          </button>
        </div>
      </form>
    </section>
  );
}
