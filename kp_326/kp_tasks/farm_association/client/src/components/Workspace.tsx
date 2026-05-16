import { ClipboardList, Database, Leaf, LogOut, ShieldCheck } from "lucide-react";
import { useEffect, useMemo, useState } from "react";

import type { FarmApiClient } from "../api/farmApi";
import {
  canMutateModule,
  getAccessibleModules,
  getRoleTitle,
} from "../domain/roles";
import type { ClientSession } from "../state/session";
import { Metric } from "./Metric";
import { moduleIcons } from "./moduleIcons";
import { ResourcePanel } from "./ResourcePanel";

interface WorkspaceProps {
  api: FarmApiClient;
  session: ClientSession;
  onLogout: () => void;
}

export function Workspace({ api, session, onLogout }: WorkspaceProps) {
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
