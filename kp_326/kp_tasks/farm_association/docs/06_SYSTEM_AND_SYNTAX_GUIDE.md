# Устройство Системы И Синтаксис

Документ объясняет, как работает система фермерской ассоциации, как связаны сервер, клиент и база данных, а также какой синтаксис и стиль используются при написании backend и frontend кода.

## Общая Модель

Система состоит из трех основных частей:

```text
React + TypeScript client
  -> HTTP JSON API
  -> C++20 server на Boost.Asio/Beast
  -> PostgreSQL
```

Клиент отвечает за интерфейс, роли, формы, таблицы, поиск и фильтрацию уже загруженных строк. Сервер отвечает за HTTP API, авторизацию, управление пользователями, CRUD предметных таблиц и подготовку базы данных. PostgreSQL хранит предметные данные фермерской ассоциации и пользователей приложения.

## Как Работает Запуск

Порядок запуска backend:

1. `server/src/Main.cpp` читает environment variables и CLI options.
2. `database::prepareDatabase` готовит PostgreSQL базу.
3. `Database::create` открывает подключение к целевой базе.
4. Создаются `PasswordHasher` и `JwtService`.
5. `UserController` проверяет или создает builtin администратора.
6. Регистрируются auth routes, user routes и resource routes.
7. `Server::run` запускает Boost.Asio/Beast HTTP server.

Упрощенная схема:

```text
main()
  -> runApplication()
    -> settingsFromEnvAndCli()
    -> prepareDatabase()
    -> Database::create()
    -> ensureBuiltinAdministrator()
    -> server.get/post/put/del(...)
    -> registerFarmEntityRoutes()
    -> server.run(settings)
```

## Как Работает Сервер

### Слои Backend

Backend разделен на слои:

| Слой | Каталог | Назначение |
| --- | --- | --- |
| Core HTTP | `server/include/server/core`, `server/src/server/core` | Beast session, router, dispatcher, request/response |
| Handling | `server/include/handling`, `server/src/handling` | Привязка маршрутов к HTTP controllers, проверка JWT, выбор HTTP status |
| HTTP controllers | `server/include/controllers/http`, `server/src/controllers/http` | Разбор JSON/query, преобразование app result в view result |
| App controllers | `server/include/controllers/app`, `server/src/controllers/app` | Бизнес-логика и работа с database facade |
| DTO | `server/include/controllers/dto` | Внутренние команды и результаты application layer |
| Views | `server/include/views` | Публичная форма ответа API |
| Marshalling | `server/include/marshalling` | `to_json`, `from_json`, преобразование entity/DTO/view |
| Persistence | `server/include/persistence`, `server/generated/persistence` | C++ представление таблиц и ODB-generated код |
| Database | `server/include/database`, `server/src/database` | Bootstrap, ODB connection, ODB table registry, низкоуровневый SQL для bootstrap |
| Security | `server/include/security`, `server/src/security` | JWT, password hash, encoding helpers |

### Маршрут Запроса

Обычный resource-запрос проходит так:

```text
Boost.Beast request
  -> RequestDispatcher
  -> AppRouter
  -> Handler
  -> HTTP controller
  -> Application controller
  -> Database
  -> PostgreSQL
```

Ответ возвращается обратно в обратном порядке:

```text
PostgreSQL row
  -> ODB persistence entity
  -> DTO
  -> View
  -> nlohmann::json
  -> HttpResponse
  -> Beast response
```

### Auth И Роли

Основные маршруты:

```text
POST /auth/register
POST /auth/login
POST /users
GET  /users
PUT  /users/role?login=<login>
```

`/auth/register` создает пользователя с ролью `farm_worker`. Управление пользователями доступно ролям `agriculture_admin` и `association_director`.

Auth-ответ:

```json
{
  "token": "<jwt>",
  "token_type": "Bearer",
  "user": {
    "login": "admin",
    "role": "agriculture_admin"
  }
}
```

JWT содержит claim `sub` для логина и claim `role` для роли. Клиент хранит токен в сессии и отправляет его в `Authorization: Bearer <token>`.

### Builtin Администратор

При старте сервер создает пользователя:

```text
login:    admin
password: admin12345
role:     agriculture_admin
```

Настройки:

```text
FARM_ADMIN_ENABLED=0
FARM_ADMIN_LOGIN=<login>
FARM_ADMIN_PASSWORD=<password>
```

### Resource CRUD

Для предметных таблиц используется единый API:

```text
GET    /api/<resource>
POST   /api/<resource>
GET    /api/<resource>/item?<key>
PUT    /api/<resource>/item?<key>
DELETE /api/<resource>/item?<key>
```

Пример списка:

```bash
curl http://localhost:8080/api/farm
```

Типовой ответ списка:

```json
{
  "resource": "farm",
  "rows": [
    {
      "id": 1,
      "name": "Ферма-001",
      "status": "active"
    }
  ]
}
```

Типовой ответ изменения:

```json
{
  "resource": "farm",
  "affectedRows": 1
}
```

### Bootstrap Базы Данных

Сервер готовит базу сам. Основные настройки:

```text
FARM_DB_USER
FARM_DB_PASSWORD
FARM_DB_HOST
FARM_DB_PORT
FARM_DB_NAME
FARM_DB_MAINTENANCE_NAME
FARM_DB_BOOTSTRAP
FARM_DB_RESET_ON_START
FARM_DB_SCHEMA_SQL
FARM_DB_USER_SQL
```

В Debug по умолчанию используется `fasc_test`, в Release - `fasc_db`. Новые базы создаются как UTF-8, а подключения libpq/ODB используют `client_encoding=UTF8`.

## Backend Синтаксис И Стиль

### C++20 Namespaces

Все серверные классы находятся в `fasc::server::<layer>`:

```cpp
namespace fasc::server::controllers::app {

class FarmController {
public:
  explicit FarmController(fasc::server::database::Database& db);
};

} // namespace fasc::server::controllers::app
```

Вспомогательные общие типы могут находиться в `fasc::extended::<group>`:

```cpp
namespace fasc::extended::fpp {
template <typename Success, typename Error>
class Result;
}
```

### Headers И Includes

Headers используют `#pragma once`. Include paths пишутся от проектных include-корней:

```cpp
#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

#include <string>
#include <vector>
```

Сначала идут проектные include, затем внешние и стандартные, если локальный файл уже следует такому стилю.

### Doxygen Комментарии

Публичные классы, методы и важные поля документируются коротко:

```cpp
/// Загружает список строк.
///@returns result со списком строк или ошибкой.
FarmRowsResult list() const;
```

Комментарий должен объяснять контракт или неочевидное поведение. Не нужно пересказывать очевидную строку кода.

### Result Вместо Исключений В Application Layer

Application controllers возвращают `Result<Success, Error>`:

```cpp
using FarmRowsResult =
    fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmRowsDto,
                                FarmEntityError>;

FarmRowsResult FarmController::list() const {
  try {
    return FarmRowsResult::success(...);
  } catch (const std::exception& exception) {
    return FarmRowsResult::error(
        FarmEntityError{FarmEntityErrorCode::DatabaseError, exception.what()});
  }
}
```

Идея: application layer не знает про HTTP status и JSON. Он возвращает предметный успех или предметную ошибку.

### HTTP Controller

HTTP controller разбирает JSON и query, вызывает app controller и возвращает view result:

```cpp
FarmRowsViewResult FarmHttpController::list() const {
  const auto result = controller_.list();
  if (result.hasError()) {
    return FarmRowsViewResult::error(toErrorView(result.error()));
  }

  return FarmRowsViewResult::success(toView(result.success().rows));
}
```

### Handler

Handler работает ближе всего к HTTP:

```cpp
HttpResponse FarmHandler::list(const HttpRequest& request) const {
  const auto result = controller_.list(request);
  return responseFrom(result, 200);
}
```

Здесь выбирается status code, сериализуется `View -> JSON`, добавляются headers.

### JSON Через nlohmann/json

Для входного JSON используется `from_json`:

```cpp
inline void from_json(const nlohmann::json& json, FarmCreateDto& value) {
  if (json.contains("name") && !json.at("name").is_null()) {
    value.name = json.at("name").get<std::string>();
  }
}
```

Для ответа используется `to_json`:

```cpp
inline void to_json(nlohmann::json& json, const FarmRowsView& view) {
  json = nlohmann::json::object();
  json["resource"] = "farm";
  json["rows"] = nlohmann::json::array();
}
```

Правило проекта: наружу идет `View -> JSON`, не прямой `DTO -> JSON`.

### Nullable Значения

В persistence часто используется `odb::nullable<T>`, а во views - `std::optional<T>`:

```cpp
if (entity.name.null()) {
  view.name = std::nullopt;
} else {
  view.name = entity.name.get();
}
```

JSON для пустого значения возвращает `null`.

### ODB Mapping

Каждая persistence-модель участвует в ODB generation:

```cpp
#pragma db object(FarmEntity) table("farm")
#pragma db member(FarmEntity::id) id auto column("id")
```

Для таблиц с составным первичным ключом используется отдельный value key и virtual id, чтобы C++ entity сохраняла привычные поля JSON/API, а ODB видел настоящий ключ таблицы:

```cpp
#pragma db value(AssociationFarmsKey)
#pragma db member(AssociationFarmsKey::farmId) column("farm_id")
#pragma db member(AssociationFarmsKey::associationId) column("association_id")

#pragma db object(AssociationFarmsEntity) table("association_farms")
#pragma db member(AssociationFarmsEntity::id) virtual(AssociationFarmsKey) access(id) id column("")
```

`domain::Date` мапится на PostgreSQL `DATE`, а enum-типы домена - на текстовые значения из SQL dump (`active`, `draft`, `RUB` и т.д.).

### Database Facade

Предметные CRUD-контроллеры используют generic методы:

```cpp
db_.selectRows("public.farm", columns);
db_.insertRow("public.farm", columns, values);
db_.updateRows("public.farm", columns, values, keys, keyValues(key));
db_.deleteRows("public.farm", keys, keyValues(key));
```

Эти методы больше не собирают SQL в контроллерах. Для зарегистрированных предметных таблиц они переходят в `OdbTableRegistry`, где table name связывается с ODB entity из `server/include/persistence` и generated mapping из `server/generated/persistence`. Поэтому обычные ключи и составные ключи обновляются/удаляются через ODB object identity.

Низкоуровневые `querySql` и `executeSql` остаются в `Database` для bootstrap, служебной диагностики и случаев, где нужен сырой SQL поверх libpq connection.

### CMake Синтаксис

Сервер собирается через CMake + Ninja:

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build --parallel
```

В `server/CMakeLists.txt` используются:

```cmake
add_library(farm_association_server_core STATIC ...)
target_include_directories(farm_association_server_core PUBLIC include)
target_link_libraries(farm_association_server_core PUBLIC fmt::fmt nlohmann_json::nlohmann_json)
add_executable(farm_association_server src/Main.cpp)
target_link_libraries(farm_association_server PRIVATE farm_association_server_core)
```

На Windows используются локальные bundled PostgreSQL/OpenSSL. На Manjaro используются системные `OpenSSL::Crypto` и `PostgreSQL::PostgreSQL`.

### SQL Синтаксис

Предметная схема находится в `database/dump-mydb-sprinthost-public.sql`. Типичные конструкции:

```sql
CREATE TABLE public.farm (
    id integer NOT NULL,
    name character varying(255),
    status character varying(32)
);

ALTER TABLE public.farm
    ADD CONSTRAINT farm_pkey PRIMARY KEY (id);
```

Серверные контроллеры ожидают реальные имена колонок из SQL dump: `legal_name`, `farm_type`, `actual_address` и так далее.

## Как Работает Клиент

### Frontend Слои

| Слой | Каталог | Назначение |
| --- | --- | --- |
| API | `client/src/api` | HTTP client, типы API, обработка ошибок |
| Domain | `client/src/domain` | Роли, модули, поля таблиц, фильтрация |
| Components | `client/src/components` | React UI: auth, workspace, таблицы, формы |
| State | `client/src/state` | Сохранение и восстановление сессии |
| Utils | `client/src/utils` | Общие helper-функции |
| Styles | `client/src/index.css` | CSS всего интерфейса |

### Поток Работы Клиента

```text
main.tsx
  -> App
    -> loadSession()
    -> AuthScreen или Workspace
      -> FarmApiClient
      -> ResourcePanel/UserAdministrationPanel
      -> DataTable/ResourceForm
```

После логина клиент получает token и user:

```ts
interface AuthResponse {
  token: string;
  token_type: string;
  user: AuthenticatedUser;
}
```

Сессия сохраняется в `localStorage`, а token устанавливается в `FarmApiClient`.

### Роли И Модули

Роли и бизнес-модули описаны в `client/src/domain/roles.ts`. Основные функции:

```ts
canAccessModule(role, module);
canMutateModule(role, module);
canManageUsers(role);
getAccessibleModules(role);
```

`agriculture_admin` видит все модули. `agriculture_admin` и `association_director` могут управлять пользователями.

### Таблицы, Поиск И Фильтры

`ResourcePanel` загружает строки через:

```ts
api.listResource(module.endpoint);
```

Затем строит колонки:

```ts
const tableColumns = columnsFor(module, rows);
const filterColumns = allColumnsFor(module, rows);
```

Фильтрация выполняется на клиенте:

```ts
const filteredRows = filterResourceRows(rows, filterColumns, searchQuery, filters);
```

Поддерживаемые операторы:

```text
contains   - Содержит
equals     - Равно
not_equals - Не равно
empty      - Пусто
not_empty  - Заполнено
```

Поиск и фильтры применяются к уже загруженным строкам. Сервер пока не получает query-параметры поиска.

## Frontend Синтаксис И Стиль

### TypeScript Types И Interfaces

API формы описываются через `interface`:

```ts
export interface AuthenticatedUser {
  login: string;
  role: UserRole;
}
```

Универсальная строка таблицы:

```ts
export type ResourceRow = Record<string, unknown>;
```

Union type используется для ограниченных значений:

```ts
export type ResourceFilterOperator =
  | "contains"
  | "equals"
  | "not_equals"
  | "empty"
  | "not_empty";
```

### React Function Components

Компоненты пишутся как функции:

```tsx
interface DataTableProps {
  columns: string[];
  rows: ResourceRow[];
}

export function DataTable({ columns, rows }: DataTableProps) {
  return <table>{/* ... */}</table>;
}
```

Props типизируются через `interface`, а компонент экспортируется именованным экспортом.

### Hooks

Состояние:

```tsx
const [searchQuery, setSearchQuery] = useState("");
const [filters, setFilters] = useState<ResourceFilter[]>([]);
```

Загрузка данных:

```tsx
const loadRows = useCallback(async () => {
  const response = await api.listResource(module.endpoint);
  setRows(response.rows);
}, [api, module.endpoint]);

useEffect(() => {
  void loadRows();
}, [loadRows]);
```

Производные значения:

```tsx
const filteredRows = useMemo(
  () => filterResourceRows(rows, filterColumns, searchQuery, filters),
  [filterColumns, filters, rows, searchQuery],
);
```

### JSX Events

Формы и input обрабатываются через React events:

```tsx
function addFilter(event: FormEvent<HTMLFormElement>) {
  event.preventDefault();
  setFilters((current) => [...current, nextFilter]);
}

<input
  onChange={(event) => setSearchQuery(event.target.value)}
  value={searchQuery}
/>
```

### HTTP Client

`FarmApiClient` оборачивает `fetch`:

```ts
const response = await fetch(joinUrl(this.baseUrl, path), {
  ...init,
  headers,
});

if (!response.ok) {
  throw new ApiRequestError(response.status, await readErrorMessage(response));
}
```

Когда token установлен, клиент добавляет:

```text
Authorization: Bearer <token>
```

### CSS

Стили находятся в `client/src/index.css`. Используется обычный CSS с классами:

```css
.table-tools {
  display: grid;
  gap: 12px;
}

.filter-chip {
  display: inline-flex;
  align-items: center;
}
```

Компонент указывает class name:

```tsx
<div className="table-tools" aria-label="Поиск и фильтрация записей">
```

### Тесты Клиента

Vitest используется для domain logic:

```ts
import { describe, expect, it } from "vitest";

describe("resource table filters", () => {
  it("searches across configured columns", () => {
    expect(filterResourceRows(rows, ["name"], "ферма", [])).toEqual([rows[0]]);
  });
});
```

Проверки:

```bash
npm run typecheck
npm test
npm run build
```

## Как Добавлять Или Менять Возможности

### Добавить Новый Backend Resource

Если таблица уже есть в SQL dump и для нее есть generated controller files:

1. Проверить persistence, DTO, views, marshalling, app controller, HTTP controller, handler.
2. Убедиться, что route регистрируется в `registerFarmEntityRoutes`.
3. Проверить `GET /api/<resource>`.
4. Добавить client module в `client/src/domain/roles.ts`.
5. Добавить default columns и права доступа.
6. Запустить CTest и client checks.

Если таблицы нет в SQL dump, сначала нужно изменить SQL-схему и bootstrap seed-данные.

### Добавить Новый Клиентский Модуль

В `client/src/domain/roles.ts` добавить объект `BusinessModule`:

```ts
{
  id: "farms",
  title: "Хозяйства",
  endpoint: "/api/farm",
  defaultColumns: ["id", "name", "status"],
  keyFields: ["id"],
  visibleBy: ["farm_owner"],
  writableBy: ["farm_owner"],
}
```

После этого `Workspace` сам покажет модуль пользователям с нужной ролью. `ResourcePanel` сам загрузит таблицу, покажет формы, поиск и фильтры.

### Добавить Новое Поле В Форму

Форма строится из `formColumnsFor(module, rows)`. Если поле должно быть видно даже до загрузки строк, добавь его в `defaultColumns` модуля. Если поле входит в первичный ключ, добавь его в `keyFields`.

### Добавить Новую Роль

Нужно синхронно обновить backend и frontend:

1. Backend: список допустимых ролей в users controller/dto.
2. Backend: правила доступа, если роль может управлять пользователями.
3. Frontend: `roleDefinitions` в `client/src/domain/roles.ts`.
4. Frontend: `visibleBy`, `writableBy`, `canManageUsers`.
5. README и client specification.
6. Тесты ролей.

### Добавить Новый Фильтр Таблицы

1. Расширить `ResourceFilterOperator`.
2. Добавить оператор в `resourceFilterOperators`.
3. Обновить `filterRequiresValue`, если оператор не требует значения.
4. Добавить case в `rowMatchesFilter`.
5. Добавить тест в `resourceFilters.test.ts`.

## Проверка После Изменений

Backend:

```bash
cmake --build server/build --parallel
ctest --test-dir server/build --output-on-failure
```

Client:

```bash
cd client
npm run typecheck
npm test
npm run build
```

Smoke-проверка UI:

1. Запустить PostgreSQL.
2. Запустить C++ server.
3. Запустить `npm run dev`.
4. Открыть `http://127.0.0.1:5173`.
5. Войти как `admin` / `admin12345`.
6. Проверить таблицу `Хозяйства`, поиск, фильтр и сброс.
7. Проверить экран `Пользователи`: список, создание пользователя, изменение роли.
