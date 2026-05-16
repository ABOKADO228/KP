import type { UserRole } from "../domain/roles";

const defaultApiBaseUrl =
  import.meta.env.VITE_API_BASE_URL ?? "";

export type ResourceRow = Record<string, unknown>;

export interface AuthenticatedUser {
  login: string;
  role: UserRole;
}

export interface AuthResponse {
  token: string;
  token_type: string;
  user: AuthenticatedUser;
}

export interface UserListResponse {
  users: AuthenticatedUser[];
}

export interface Credentials {
  login: string;
  password: string;
}

export interface CreateUserRequest extends Credentials {
  role: UserRole;
}

export interface ResourceListResponse<T extends ResourceRow = ResourceRow> {
  resource: string;
  rows: T[];
}

export interface ResourceMutationResponse {
  resource: string;
  affectedRows: number;
}

export class ApiRequestError extends Error {
  readonly status: number;

  constructor(status: number, message: string) {
    super(message);
    this.name = "ApiRequestError";
    this.status = status;
  }
}

export class FarmApiClient {
  private token: string | null = null;

  constructor(private readonly baseUrl: string = defaultApiBaseUrl) {}

  setToken(token: string | null): void {
    this.token = token;
  }

  login(credentials: Credentials): Promise<AuthResponse> {
    return this.post<AuthResponse>("/auth/login", credentials);
  }

  register(credentials: Credentials): Promise<AuthResponse> {
    return this.post<AuthResponse>("/auth/register", credentials);
  }

  createUser(user: CreateUserRequest): Promise<AuthenticatedUser> {
    return this.post<AuthenticatedUser>("/users", user);
  }

  listUsers(): Promise<UserListResponse> {
    return this.request<UserListResponse>("/users");
  }

  updateUserRole(login: string, role: UserRole): Promise<AuthenticatedUser> {
    const searchParams = new URLSearchParams({ login });
    return this.request<AuthenticatedUser>(`/users/role?${searchParams.toString()}`, {
      method: "PUT",
      body: JSON.stringify({ role }),
    });
  }

  listResource<T extends ResourceRow = ResourceRow>(
    endpoint: string,
  ): Promise<ResourceListResponse<T>> {
    return this.request<ResourceListResponse<T>>(endpoint);
  }

  createResource(
    endpoint: string,
    payload: ResourceRow,
  ): Promise<ResourceMutationResponse> {
    return this.post<ResourceMutationResponse>(endpoint, payload);
  }

  updateResource(
    endpoint: string,
    key: ResourceRow,
    payload: ResourceRow,
  ): Promise<ResourceMutationResponse> {
    return this.request<ResourceMutationResponse>(
      `${endpoint}/item?${queryStringFrom(key)}`,
      {
        method: "PUT",
        body: JSON.stringify(payload),
      },
    );
  }

  deleteResource(
    endpoint: string,
    key: ResourceRow,
  ): Promise<ResourceMutationResponse> {
    return this.request<ResourceMutationResponse>(
      `${endpoint}/item?${queryStringFrom(key)}`,
      {
        method: "DELETE",
      },
    );
  }

  private post<TResponse>(path: string, body: unknown): Promise<TResponse> {
    return this.request<TResponse>(path, {
      method: "POST",
      body: JSON.stringify(body),
    });
  }

  private async request<TResponse>(
    path: string,
    init: RequestInit = {},
  ): Promise<TResponse> {
    const headers = new Headers(init.headers);
    if (init.body && !headers.has("Content-Type")) {
      headers.set("Content-Type", "application/json");
    }
    if (this.token) {
      headers.set("Authorization", `Bearer ${this.token}`);
    }

    const response = await fetch(joinUrl(this.baseUrl, path), {
      ...init,
      headers,
    });

    if (!response.ok) {
      throw new ApiRequestError(response.status, await readErrorMessage(response));
    }

    return (await response.json()) as TResponse;
  }
}

function joinUrl(baseUrl: string, path: string): string {
  const normalizedBase = baseUrl.endsWith("/") ? baseUrl.slice(0, -1) : baseUrl;
  const normalizedPath = path.startsWith("/") ? path : `/${path}`;
  return `${normalizedBase}${normalizedPath}`;
}

function queryStringFrom(values: ResourceRow): string {
  const searchParams = new URLSearchParams();
  for (const [key, value] of Object.entries(values)) {
    if (value !== null && value !== undefined) {
      searchParams.set(key, String(value));
    }
  }
  return searchParams.toString();
}

async function readErrorMessage(response: Response): Promise<string> {
  try {
    const payload = (await response.json()) as { error?: unknown };
    if (typeof payload.error === "string" && payload.error.trim()) {
      return payload.error;
    }
  } catch {
    // Fall through to the HTTP status text below.
  }

  return response.statusText || `HTTP ${response.status}`;
}
