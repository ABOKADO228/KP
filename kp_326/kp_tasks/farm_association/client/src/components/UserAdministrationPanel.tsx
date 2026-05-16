import { FormEvent, useEffect, useState } from "react";
import { RefreshCw, UserPlus } from "lucide-react";

import type { AuthenticatedUser, FarmApiClient } from "../api/farmApi";
import { getRoleTitle, roleDefinitions, type UserRole } from "../domain/roles";
import { errorMessage } from "../utils/errors";

interface UserAdministrationPanelProps {
  api: FarmApiClient;
}

export function UserAdministrationPanel({ api }: UserAdministrationPanelProps) {
  const [users, setUsers] = useState<AuthenticatedUser[]>([]);
  const [login, setLogin] = useState("");
  const [password, setPassword] = useState("");
  const [role, setRole] = useState<UserRole>("farm_worker");
  const [error, setError] = useState<string | null>(null);
  const [notice, setNotice] = useState<string | null>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [updatingLogin, setUpdatingLogin] = useState<string | null>(null);

  useEffect(() => {
    void loadUsers();
  }, []);

  async function loadUsers() {
    setError(null);
    setIsLoading(true);

    try {
      const response = await api.listUsers();
      setUsers(sortUsers(response.users));
    } catch (caught) {
      setError(errorMessage(caught));
    } finally {
      setIsLoading(false);
    }
  }

  async function submitUser(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setError(null);
    setNotice(null);
    setIsSubmitting(true);

    try {
      const created = await api.createUser({ login, password, role });
      setUsers((current) =>
        sortUsers([
          ...current.filter((user) => user.login !== created.login),
          created,
        ]),
      );
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

  async function updateRole(user: AuthenticatedUser, nextRole: UserRole) {
    if (user.role === nextRole) {
      return;
    }

    setError(null);
    setNotice(null);
    setUpdatingLogin(user.login);

    try {
      const updated = await api.updateUserRole(user.login, nextRole);
      setUsers((current) =>
        sortUsers(
          current.map((candidate) =>
            candidate.login === updated.login ? updated : candidate,
          ),
        ),
      );
      setNotice(`${updated.login}: ${getRoleTitle(updated.role)}`);
    } catch (caught) {
      setError(errorMessage(caught));
    } finally {
      setUpdatingLogin(null);
    }
  }

  return (
    <section className="data-panel" aria-labelledby="users-title">
      <div className="panel-header">
        <div>
          <p className="eyebrow">users</p>
          <h2 id="users-title">Пользователи</h2>
        </div>
        <div className="panel-actions">
          <button
            className="icon-button text-button"
            disabled={isLoading}
            onClick={loadUsers}
            type="button"
          >
            <RefreshCw aria-hidden="true" size={18} />
            Обновить
          </button>
          <span className="access-badge write">Управление</span>
        </div>
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

      {isLoading ? (
        <div className="empty-state">Загрузка</div>
      ) : users.length === 0 ? (
        <div className="empty-state">Нет пользователей</div>
      ) : (
        <div className="table-scroll">
          <table>
            <thead>
              <tr>
                <th>Логин</th>
                <th>Роль</th>
              </tr>
            </thead>
            <tbody>
              {users.map((user) => (
                <tr key={user.login}>
                  <td>{user.login}</td>
                  <td>
                    <select
                      aria-label={`Роль ${user.login}`}
                      disabled={updatingLogin === user.login}
                      onChange={(event) =>
                        void updateRole(user, event.target.value as UserRole)
                      }
                      value={user.role}
                    >
                      {roleDefinitions.map((definition) => (
                        <option key={definition.id} value={definition.id}>
                          {definition.title}
                        </option>
                      ))}
                    </select>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      )}
    </section>
  );
}

function sortUsers(users: AuthenticatedUser[]): AuthenticatedUser[] {
  return [...users].sort((left, right) => left.login.localeCompare(right.login));
}
