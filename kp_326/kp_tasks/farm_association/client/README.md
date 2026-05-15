# Farm Association Client

React + TypeScript клиент для HTTP API сервера из каталога `server`.

## Запуск

```powershell
cd client
npm install
npm run dev
```

По умолчанию Vite открывает приложение на `http://127.0.0.1:5173` и проксирует `/server-api/*` в C++ сервер `http://127.0.0.1:8080`.

Сервер можно переопределить:

```powershell
$env:FARM_SERVER_URL = "http://127.0.0.1:18081"
npm run dev
```

## Проверка

```powershell
npm run typecheck
npm test
npm run build
```

## Структура

- `src/api` - тонкий клиент HTTP API.
- `src/domain` - роли, бизнес-модули и правила доступа.
- `src/state` - хранение клиентской сессии.
- `src/App.tsx` - начальный role-based интерфейс.
- `SPECIFICATION.md` - продуктовая и техническая спецификация клиента.
