import react from "@vitejs/plugin-react";
import { defineConfig } from "vitest/config";

const serverTarget =
  process.env.FARM_SERVER_URL ??
  process.env.VITE_FARM_SERVER_URL ??
  "http://127.0.0.1:8080";

export default defineConfig({
  plugins: [react()],
  server: {
    port: 5173,
    strictPort: false,
    proxy: {
      "/server-api": {
        target: serverTarget,
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/server-api/, ""),
      },
    },
  },
  preview: {
    port: 4173,
  },
  test: {
    environment: "node",
  },
});
