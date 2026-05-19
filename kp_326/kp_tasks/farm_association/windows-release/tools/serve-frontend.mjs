import { createReadStream, existsSync, statSync } from "node:fs";
import { createServer } from "node:http";
import { extname, join, normalize, resolve, sep } from "node:path";
import { fileURLToPath } from "node:url";

const args = new Map();
for (let i = 2; i < process.argv.length; i += 2) {
  args.set(process.argv[i], process.argv[i + 1]);
}

const scriptDir = resolve(fileURLToPath(new URL(".", import.meta.url)));
const root = resolve(args.get("--root") ?? join(scriptDir, "..", "frontend"));
const host = args.get("--host") ?? "127.0.0.1";
const port = Number(args.get("--port") ?? "4173");

const contentTypes = new Map([
  [".css", "text/css; charset=utf-8"],
  [".html", "text/html; charset=utf-8"],
  [".js", "text/javascript; charset=utf-8"],
  [".json", "application/json; charset=utf-8"],
  [".map", "application/json; charset=utf-8"],
  [".png", "image/png"],
  [".svg", "image/svg+xml"],
  [".ico", "image/x-icon"],
  [".woff", "font/woff"],
  [".woff2", "font/woff2"],
]);

function resolveRequestPath(url) {
  const rawPath = decodeURIComponent(new URL(url, `http://${host}:${port}`).pathname);
  const relativePath = normalize(rawPath).replace(/^([/\\])+/, "");
  const candidate = resolve(root, relativePath);
  const rootWithSep = root.endsWith(sep) ? root : `${root}${sep}`;

  if (candidate !== root && !candidate.startsWith(rootWithSep)) {
    return null;
  }

  if (existsSync(candidate) && statSync(candidate).isFile()) {
    return candidate;
  }

  if (existsSync(candidate) && statSync(candidate).isDirectory()) {
    const indexPath = join(candidate, "index.html");
    if (existsSync(indexPath)) {
      return indexPath;
    }
  }

  const fallback = join(root, "index.html");
  return existsSync(fallback) ? fallback : null;
}

const server = createServer((request, response) => {
  const filePath = resolveRequestPath(request.url ?? "/");
  if (filePath === null) {
    response.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
    response.end("Not found");
    return;
  }

  const contentType = contentTypes.get(extname(filePath)) ?? "application/octet-stream";
  response.writeHead(200, {
    "Cache-Control": filePath.endsWith("index.html") ? "no-cache" : "public, max-age=31536000, immutable",
    "Content-Type": contentType,
  });
  createReadStream(filePath).pipe(response);
});

server.listen(port, host, () => {
  console.log(`frontend listening on http://${host}:${port}`);
  console.log(`serving ${root}`);
});
