# Спецификация Модуля: C++ Сервер Фермерской Ассоциации

## Назначение

Модуль `server` содержит C++20 HTTP backend. Он принимает HTTP-запросы, маршрутизирует их через Boost.Beast/Asio, сериализует JSON через nlohmann/json, работает с PostgreSQL через ODB/libpq и собирается через CMake + Ninja + clang++.

## Технологический Стек

```text
C++20
CMake + Ninja
clang++
Boost.Asio/Beast
nlohmann/json
fmt
ODB
PostgreSQL/libpq
OpenSSL/libcrypto
GoogleTest/CTest
```

## Политика Зависимостей

Зависимости устанавливаются локально внутри проекта, без vcpkg и без хранения больших библиотек в Git:

```text
server/
  third_party/
    boost/
    nlohmann_json/
    odb/
    postgresql/
    openssl/                 Manjaro
    libodb-Debug/            Windows Debug build output
    libodb-pgsql-Debug/      Windows Debug build output
    libodb-Release/          Windows Release build output
    libodb-pgsql-Release/    Windows Release build output
    _sources/
    _downloads/
    _build/
```

В Git должен оставаться только `server/third_party/.gitkeep`.

## Сборочная Модель

CMake создает:

```text
farm_association_server_core
farm_association_server
farm_association_unit_tests
farm_association_integration_tests
```

`farm_association_server_core` содержит основную логику: controllers, handlers, database layer, security, router, dispatcher и server core.

`farm_association_server` содержит `src/Main.cpp` и запускает приложение.

Тестовые targets линкуются к `farm_association_server_core`, чтобы проверять тот же код, который используется сервером.

## Слои Кода

```text
include/views
  JSON-facing структуры API. Не зависят от persistence.

include/persistence
  ODB-facing persistent classes.

src/controllers/app
  Бизнес-операции и работа через database wrapper.

src/controllers/http
  HTTP-facing controller адаптеры.

src/handling
  Endpoint handlers и регистрация CRUD routes.

include/marshalling
  Header-only JSON serialization/deserialization.

src/database
  Database wrapper и startup bootstrap PostgreSQL.

src/server/core
  Router, dispatcher и Beast server loop.
```

## ODB Workflow

ODB persistent models:

```text
server/include/persistence/*.hpp
server/generated/persistence/*-odb.cxx
server/generated/persistence/*-odb.hxx
server/generated/persistence/*-odb.ixx
```

`User` содержит `login` как primary key, `password_hash` и серверную роль `role`. Предметные persistence-структуры зеркалят таблицы SQL-дампа и тоже имеют ODB-generated mapping. Таблицы с составным первичным ключом (`association_farms`, `farm_plot_assignment`, `farm_plot_consumption_product`, `farm_plot_production_product`) описаны через composite value key и virtual ODB id.

На build этапе CMake либо запускает ODB compiler для всех моделей, либо берет checked-in generated files из `server/generated/persistence`:

```text
server/build/generated/persistence/<model>-odb.cxx
server/build/generated/persistence/<model>-odb.hxx
server/build/generated/persistence/<model>-odb.ixx
server/build/generated/persistence/<model>.sql
```

Generated `.cxx` входят в `farm_association_server_core`. Bootstrap применяет предметный SQL dump и generated `user.sql`; предметные таблицы уже создаются dump-файлом, а ODB-generated files используются runtime CRUD-операциями.

## Database Bootstrap

Сервер готовит PostgreSQL на старте:

```text
Debug
  target database: fasc_test
  behavior: drop/create/init on every server start

Release
  target database: fasc_db
  behavior: create/init only when missing
```

SQL-источники:

```text
database/dump-mydb-sprinthost-public.sql
server/build/generated/persistence/user.sql
```

Управление через окружение:

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

Обычный запуск не требует ручных `createdb` и `psql -f`: сервер создает целевую базу и применяет SQL сам, когда PostgreSQL service доступен.

## API Model

View-слой отделен от persistence. JSON endpoints работают со структурами из `include/views`, а не напрямую с ODB-классами. Это позволяет менять схему хранения без обязательного изменения внешнего API и держать маршаллинг независимым.

Ключи сущностей используют `std::uint64_t`. Поля, которые имеют фиксированный набор значений, должны быть enum-типами во view/domain-слое и маршаллиться как ожидаемые JSON значения.

Пользовательские маршруты:

```text
POST /auth/register
POST /auth/login
POST /users
GET  /users
PUT  /users/role?login=<login>
```

`/auth/register` и `/auth/login` принимают `{ "login": "...", "password": "..." }`. `/auth/register` создает пользователя с ролью `farm_worker`; управленческие маршруты `/users` требуют `Authorization: Bearer <token>` пользователя с ролью `agriculture_admin` или `association_director`. `POST /users` создает пользователя с явной ролью, `GET /users` возвращает список пользователей, `PUT /users/role?login=<login>` изменяет роль. Auth-ответ содержит JWT и пользователя:

```json
{
  "token": "<jwt>",
  "token_type": "Bearer",
  "user": {
    "login": "alex",
    "role": "farm_worker"
  }
}
```

Старт сервера обеспечивает встроенного администратора `admin` / `admin12345` с ролью `agriculture_admin`. Значения переопределяются через `FARM_ADMIN_LOGIN` и `FARM_ADMIN_PASSWORD`, создание отключается через `FARM_ADMIN_ENABLED=0`.

Предметные CRUD endpoints зарегистрированы для таблиц из `FarmEntityRoutes` по схеме:

```text
GET    /api/<resource>
POST   /api/<resource>
GET    /api/<resource>/item?<key>
PUT    /api/<resource>/item?<key>
DELETE /api/<resource>/item?<key>
```

Списки возвращают `{ "resource": "<resource>", "rows": [...] }`, одиночная строка - `{ "resource": "<resource>", "data": { ... } }`, мутации - `{ "resource": "<resource>", "affectedRows": <number> }`.

## Тестирование

Текущий набор тестов покрывает:

```text
unit/controller
unit/database
unit/marshalling
integration/endpoints
integration/business
integration/database
```

CTest запускается так:

```bash
ctest --test-dir server/build --output-on-failure
```

Unit tests не требуют живой PostgreSQL. Integration tests включают DB-backed проверки и работают через `fasc_test`: тестовый bootstrap сбрасывает базу, применяет SQL-схемы и проверяет реальные DB-backed endpoints.
