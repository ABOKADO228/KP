import type { AuthenticatedUser } from "../api/farmApi";
import { isKnownRole } from "../domain/roles";

const storageKey = "farm-association-client.session";

export interface ClientSession {
  token: string;
  tokenType: string;
  user: AuthenticatedUser;
}

export function loadSession(): ClientSession | null {
  const rawValue = window.localStorage.getItem(storageKey);
  if (!rawValue) {
    return null;
  }

  try {
    const parsed = JSON.parse(rawValue) as ClientSession;
    if (
      typeof parsed.token === "string" &&
      typeof parsed.tokenType === "string" &&
      typeof parsed.user?.login === "string" &&
      isKnownRole(parsed.user.role)
    ) {
      return parsed;
    }
  } catch {
    // Broken local state should not block opening the application.
  }

  window.localStorage.removeItem(storageKey);
  return null;
}

export function saveSession(session: ClientSession): void {
  window.localStorage.setItem(storageKey, JSON.stringify(session));
}

export function clearSession(): void {
  window.localStorage.removeItem(storageKey);
}

export function createSession(
  token: string,
  tokenType: string,
  user: AuthenticatedUser,
): ClientSession {
  return {
    token,
    tokenType,
    user,
  };
}
