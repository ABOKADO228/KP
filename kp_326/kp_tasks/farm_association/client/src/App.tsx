import {
  Building2,
  ClipboardList,
  Database,
  FileText,
  KeyRound,
  Leaf,
  LogOut,
  Package,
  RefreshCw,
  ShieldCheck,
  ShoppingCart,
  Tractor,
  Truck,
  UserPlus,
  Users,
  type LucideIcon,
} from "lucide-react";
import {
  FormEvent,
  useCallback,
  useEffect,
  useMemo,
  useState,
} from "react";

import {
  ApiRequestError,
  FarmApiClient,
  type ResourceRow,
} from "./api/farmApi";
import {
  canMutateModule,
  getAccessibleModules,
  getRoleTitle,
  roleDefinitions,
  type BusinessModule,
  type ModuleIcon,
  type UserRole,
} from "./domain/roles";
import {
  clearSession,
  createSession,
  loadSession,
  saveSession,
  type ClientSession,
} from "./state/session";

const moduleIcons: Record<ModuleIcon, LucideIcon> = {
  association: Building2,
  contract: FileText,
  farm: Tractor,
  field: Leaf,
  member: Users,
  order: ShoppingCart,
  product: Package,
  supplier: Truck,
};

function App() {
  const api = useMemo(() => new FarmApiClient(), []);
  const [session, setSession] = useState<ClientSession | null>(() => loadSession());

  useEffect(() => {
    api.setToken(session?.token ?? null);
  }, [api, session]);

  const handleSessionChange = useCallback((nextSession: ClientSession) => {
    saveSession(nextSession);
    setSession(nextSession);
  }, []);

  const handleLogout = useCallback(() => {
    clearSession();
    setSession(null);
  }, []);

  if (!session) {
    return <AuthScreen api={api} onAuthenticated={handleSessionChange} />;
  }

  return <Workspace api={api} session={session} onLogout={handleLogout} />;
}

interface AuthScreenProps {
  api: FarmApiClient;
  onAuthenticated: (session: ClientSession) => void;
}

function AuthScreen({ api, onAuthenticated }: AuthScreenProps) {
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

      onAuthenticated(
        createSession(auth.token, auth.token_type, auth.user, role),
      );
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
                setMode((current) =>
                  current === "login" ? "register" : "login",
                )
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

interface WorkspaceProps {
  api: FarmApiClient;
  session: ClientSession;
  onLogout: () => void;
}

function Workspace({ api, session, onLogout }: WorkspaceProps) {
  const modules = useMemo(
    () => getAccessibleModules(session.user.role),
    [session.user.role],
  );
  const [selectedModuleId, setSelectedModuleId] = useState(modules[0]?.id ?? "");

  useEffect(() => {
    if (!modules.some((module) => module.id === selectedModuleId)) {
      setSelectedModuleId(modules[0]?.id ?? "");
    }
  }, [modules, selectedModuleId]);

  const selectedModule =
    modules.find((module) => module.id === selectedModuleId) ?? modules[0];

  return (
    <main className="app-shell">
      <aside className="sidebar" aria-label="Разделы">
        <div className="sidebar-brand">
          <Leaf aria-hidden="true" size={24} />
          <span>FASC</span>
        </div>

        <nav className="module-nav">
          {modules.map((module) => {
            const Icon = moduleIcons[module.icon];
            const isActive = module.id === selectedModule?.id;

            return (
              <button
                aria-current={isActive ? "page" : undefined}
                className={isActive ? "module-link active" : "module-link"}
                key={module.id}
                onClick={() => setSelectedModuleId(module.id)}
                type="button"
              >
                <Icon aria-hidden="true" size={18} />
                <span>{module.title}</span>
              </button>
            );
          })}
        </nav>
      </aside>

      <section className="workspace">
        <header className="topbar">
          <div>
            <p className="eyebrow">{getRoleTitle(session.user.role)}</p>
            <h1>{session.user.name}</h1>
          </div>
          <button className="icon-button text-button" onClick={onLogout} type="button">
            <LogOut aria-hidden="true" size={18} />
            Выйти
          </button>
        </header>

        <section className="metric-grid" aria-label="Сводка">
          <Metric icon={Database} label="Разделы" value={modules.length} tone="blue" />
          <Metric
            icon={ShieldCheck}
            label="Запись"
            value={
              modules.filter((module) =>
                canMutateModule(session.user.role, module),
              ).length
            }
            tone="green"
          />
          <Metric
            icon={ClipboardList}
            label="Процессы"
            value={new Set(modules.map((module) => module.area)).size}
            tone="amber"
          />
        </section>

        {selectedModule ? (
          <ResourcePanel
            api={api}
            module={selectedModule}
            role={session.user.role}
          />
        ) : (
          <section className="empty-state">Нет доступных разделов</section>
        )}
      </section>
    </main>
  );
}

interface MetricProps {
  icon: LucideIcon;
  label: string;
  value: number;
  tone: "amber" | "blue" | "green";
}

function Metric({ icon: Icon, label, value, tone }: MetricProps) {
  return (
    <article className={`metric ${tone}`}>
      <Icon aria-hidden="true" size={20} />
      <div>
        <span>{label}</span>
        <strong>{value}</strong>
      </div>
    </article>
  );
}

interface ResourcePanelProps {
  api: FarmApiClient;
  module: BusinessModule;
  role: UserRole;
}

function ResourcePanel({ api, module, role }: ResourcePanelProps) {
  const [rows, setRows] = useState<ResourceRow[]>([]);
  const [resourceName, setResourceName] = useState(module.id);
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

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

  const writable = canMutateModule(role, module);

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
          <button
            className="icon-button"
            disabled={isLoading}
            onClick={() => void loadRows()}
            title="Обновить"
            type="button"
          >
            <RefreshCw aria-hidden="true" size={18} />
          </button>
        </div>
      </div>

      {error ? <div className="error-banner">{error}</div> : null}

      <DataTable columns={columnsFor(module, rows)} isLoading={isLoading} rows={rows} />
    </section>
  );
}

interface DataTableProps {
  columns: string[];
  isLoading: boolean;
  rows: ResourceRow[];
}

function DataTable({ columns, isLoading, rows }: DataTableProps) {
  if (isLoading) {
    return <div className="empty-state">Загрузка</div>;
  }

  if (rows.length === 0) {
    return <div className="empty-state">Нет данных</div>;
  }

  return (
    <div className="table-scroll">
      <table>
        <thead>
          <tr>
            {columns.map((column) => (
              <th key={column}>{column}</th>
            ))}
          </tr>
        </thead>
        <tbody>
          {rows.map((row, index) => (
            <tr key={rowKey(row, index)}>
              {columns.map((column) => (
                <td key={column}>{formatValue(row[column])}</td>
              ))}
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

function columnsFor(module: BusinessModule, rows: ResourceRow[]): string[] {
  const rowColumns = rows.flatMap((row) => Object.keys(row));
  const uniqueColumns = new Set([...module.defaultColumns, ...rowColumns]);
  return Array.from(uniqueColumns).slice(0, 10);
}

function rowKey(row: ResourceRow, index: number): string {
  const id = row.id ?? row.farm_id ?? row.product_id ?? row.association_id;
  return `${String(id ?? "row")}-${index}`;
}

function formatValue(value: unknown): string {
  if (value === null || value === undefined) {
    return "—";
  }
  if (typeof value === "boolean") {
    return value ? "Да" : "Нет";
  }
  if (typeof value === "object") {
    return JSON.stringify(value);
  }
  return String(value);
}

function errorMessage(caught: unknown): string {
  if (caught instanceof ApiRequestError) {
    return `${caught.status}: ${caught.message}`;
  }
  if (caught instanceof Error) {
    return caught.message;
  }
  return "Неизвестная ошибка";
}

export default App;
