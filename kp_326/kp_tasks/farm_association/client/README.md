# Farm Association Client

React + TypeScript клиент для HTTP API сервера из каталога `server`. Текущий интерфейс включает авторизацию/регистрацию, боковую навигацию по доступным бизнес-модулям роли пользователя, администраторский экран создания пользователей с выбором роли, таблицу ресурса, формы создания и редактирования, подтверждение удаления и сохранение сессии в `localStorage`.

## Запуск

```powershell
cd client
npm install
npm run dev
```

По умолчанию Vite открывает приложение на `http://127.0.0.1:5173`, а API-клиент обращается к C++ серверу напрямую по `http://127.0.0.1:8080`. Backend можно переопределить через `VITE_FARM_SERVER_URL` или `VITE_API_BASE_URL`; сервер возвращает CORS/preflight-заголовки для dev-режима.

Сервер можно переопределить:

```powershell
$env:VITE_FARM_SERVER_URL = "http://127.0.0.1:18081"
npm run dev
```

Серверные запросы выполняются через `FarmApiClient`: `POST /auth/login`, `POST /auth/register`, защищенный `POST /users` для администратора, `GET /api/<resource>`, `POST /api/<resource>`, `PUT /api/<resource>/item?<key>` и `DELETE /api/<resource>/item?<key>`.

## Проверка

```powershell
npm run typecheck
npm test
npm run build
```

## Структура

- `src/api` - тонкий клиент HTTP API.
- `src/components` - экран авторизации, рабочая область, администрирование пользователей, таблица, формы и подтверждение удаления.
- `src/domain` - роли, бизнес-модули, ключевые поля и правила доступа.
- `src/state` - хранение клиентской сессии.
- `src/utils` - преобразование ошибок API в пользовательский текст.
- `src/App.tsx` - корневое переключение между авторизацией и рабочей областью.
- `SPECIFICATION.md` - продуктовая и техническая спецификация клиента.
