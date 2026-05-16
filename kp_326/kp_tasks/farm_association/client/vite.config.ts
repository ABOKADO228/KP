import react from "@vitejs/plugin-react";
import { loadEnv } from "vite";
import { defineConfig } from "vitest/config";

export default defineConfig(({ mode }) => {
  const env = loadEnv(mode, process.cwd(), "");
  const backendUrl =
    env.FARM_SERVER_URL ??
    env.VITE_FARM_SERVER_URL ??
    env.VITE_API_BASE_URL ??
    "http://127.0.0.1:8080";
  const proxyTarget = {
    changeOrigin: true,
    target: backendUrl,
  };

  return {
    plugins: [react()],
    server: {
      port: 5173,
      strictPort: false,
      proxy: {
        "/api": proxyTarget,
        "/auth": proxyTarget,
        "/health": proxyTarget,
        "/users": proxyTarget,
      },
    },
    preview: {
      port: 4173,
    },
    test: {
      environment: "node",
    },
  };
});
