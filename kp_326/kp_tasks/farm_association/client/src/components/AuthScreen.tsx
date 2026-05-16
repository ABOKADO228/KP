import { KeyRound, Leaf, UserPlus } from "lucide-react";
import { FormEvent, useState } from "react";

import type { FarmApiClient } from "../api/farmApi";
import { createSession, type ClientSession } from "../state/session";
import { errorMessage } from "../utils/errors";

interface AuthScreenProps {
  api: FarmApiClient;
  onAuthenticated: (session: ClientSession) => void;
}

export function AuthScreen({ api, onAuthenticated }: AuthScreenProps) {
  const [mode, setMode] = useState<"login" | "register">("login");
  const [login, setLogin] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [isSubmitting, setIsSubmitting] = useState(false);

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setError(null);
    setIsSubmitting(true);

    try {
      const auth =
        mode === "login"
          ? await api.login({ login, password })
          : await api.register({ login, password });

      onAuthenticated(createSession(auth.token, auth.token_type, auth.user));
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
            <span>Логин</span>
            <input
              autoComplete="username"
              name="login"
              onChange={(event) => setLogin(event.target.value)}
              required
              value={login}
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
