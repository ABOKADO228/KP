# Farm Association Client

React + TypeScript клиент для HTTP API сервера из каталога `server`. Текущий интерфейс включает авторизацию/регистрацию, боковую навигацию по доступным бизнес-модулям роли пользователя, экран управления пользователями для `agriculture_admin` и `association_director`, таблицу ресурса с клиентским поиском и фильтрами, формы создания и редактирования, подтверждение удаления и сохранение сессии в `localStorage`.

## Запуск

```powershell
cd client
npm install
npm run dev
```

По умолчанию Vite открывает приложение на `http://127.0.0.1:5173` и проксирует `/auth`, `/users`, `/api` и `/health` в C++ сервер `http://127.0.0.1:8080`. Backend для dev-прокси можно переопределить через `FARM_SERVER_URL`; сервер также возвращает CORS/preflight-заголовки для прямых клиентов.

Сервер можно переопределить:

```powershell
$env:FARM_SERVER_URL = "http://127.0.0.1:18081"
npm run dev
```

Серверные запросы выполняются через `FarmApiClient`: `POST /auth/login`, `POST /auth/register`, защищенные `POST /users`, `GET /users`, `PUT /users/role?login=<login>` для управленческих ролей, `GET /api/<resource>`, `POST /api/<resource>`, `PUT /api/<resource>/item?<key>` и `DELETE /api/<resource>/item?<key>`.

## Проверка

```powershell
npm run typecheck
npm test
npm run build
```

## Структура

- `src/api` - тонкий клиент HTTP API.
- `src/components` - экран авторизации, рабочая область, администрирование пользователей, таблица, поиск/фильтры, формы и подтверждение удаления.
- `src/domain` - роли, бизнес-модули, ключевые поля, правила доступа и клиентская фильтрация строк.
- `src/state` - хранение клиентской сессии.
- `src/utils` - преобразование ошибок API в пользовательский текст.
- `src/App.tsx` - корневое переключение между авторизацией и рабочей областью.
- `SPECIFICATION.md` - продуктовая и техническая спецификация клиента.

Общий обзор архитектуры, синтаксиса React/TypeScript и правил добавления модулей описан в `../docs/06_SYSTEM_AND_SYNTAX_GUIDE.md`.
