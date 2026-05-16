import { useCallback, useEffect, useMemo, useState } from "react";

import { FarmApiClient } from "./api/farmApi";
import { AuthScreen } from "./components/AuthScreen";
import { Workspace } from "./components/Workspace";
import {
  clearSession,
  loadSession,
  saveSession,
  type ClientSession,
} from "./state/session";

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

export default App;
