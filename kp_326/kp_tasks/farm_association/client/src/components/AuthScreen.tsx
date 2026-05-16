import { KeyRound, Leaf, UserPlus } from "lucide-react";
import { FormEvent, useState } from "react";

import type { FarmApiClient } from "../api/farmApi";
import { roleDefinitions, type UserRole } from "../domain/roles";
import { createSession, type ClientSession } from "../state/session";
import { errorMessage } from "../utils/errors";

interface AuthScreenProps {
  api: FarmApiClient;
  onAuthenticated: (session: ClientSession) => void;
}

export function AuthScreen({ api, onAuthenticated }: AuthScreenProps) {
  const [mode, setMode] = useState<"login" | "register">("login");
  const [name, setName] = useState("");
  const [password, setPassword] = useState("");
  const [role, setRole] = useState<UserRole>("agriculture_admin");
  const [error, setError] = useState<string | null>(null);
  const [isSubmitting, setIsSubmitting] = useState(false);

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setError(null);
    setIsSubmitting(true);

    try {
      const auth =
        mode === "login"
          ? await api.login({ name, password })
          : await api.register({ name, password });

      onAuthenticated(createSession(auth.token, auth.token_type, auth.user, role));
    } catch (caught) {
      setError(errorMessage(caught));
    } finally {
      setIsSubmitting(false);
    }
  }

  return (
    <main className="auth-page">
      <section className="auth-panel" aria-labelledby="auth-title">
        <div className="brand-mark">
          <Leaf aria-hidden="true" size={24} />
        </div>
        <div>
          <p className="eyebrow">Farm Association</p>
          <h1 id="auth-title">Рабочий кабинет ассоциации</h1>
        </div>

        <form className="auth-form" onSubmit={handleSubmit}>
          <label>
            <span>Пользователь</span>
            <input
              autoComplete="username"
              name="name"
              onChange={(event) => setName(event.target.value)}
              required
              value={name}
            />
          </label>

          <label>
            <span>Пароль</span>
            <input
              autoComplete={mode === "login" ? "current-password" : "new-password"}
              minLength={8}
              name="password"
              onChange={(event) => setPassword(event.target.value)}
              required
              type="password"
              value={password}
            />
          </label>

          <label>
            <span>Роль</span>
            <select
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

          {error ? <p className="form-error">{error}</p> : null}

          <div className="auth-actions">
            <button className="primary-button" disabled={isSubmitting} type="submit">
              {mode === "login" ? (
                <KeyRound aria-hidden="true" size={18} />
              ) : (
                <UserPlus aria-hidden="true" size={18} />
              )}
              {mode === "login" ? "Войти" : "Создать"}
            </button>
            <button
              className="ghost-button"
              onClick={() =>
                setMode((current) => (current === "login" ? "register" : "login"))
              }
              type="button"
            >
              {mode === "login" ? "Регистрация" : "Вход"}
            </button>
          </div>
        </form>
      </section>
    </main>
  );
}
