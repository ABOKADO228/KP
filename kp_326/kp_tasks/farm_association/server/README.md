# Server

Этот каталог содержит C++20 backend для `farm_association`.

Поддерживаемая схема сборки:

```text
CMake + Ninja + clang++
```

Перед сборкой подготовь зависимости из корня репозитория:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
```

```bash
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh
```

Debug:

```powershell
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server\build --config Debug --parallel
$pg = "$PWD\server\third_party\postgresql"
if (!(Test-Path "$pg\data\PG_VERSION")) { & "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust --encoding=UTF8 --locale=C }
& "$pg\bin\pg_isready.exe" -h localhost -p 5432
if ($LASTEXITCODE -ne 0) { & "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start }
ctest --test-dir server\build -C Debug --output-on-failure
```

Release:

```powershell
cmake -S server -B server\build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build server\build-release --config Release --parallel
$pg = "$PWD\server\third_party\postgresql"
if (!(Test-Path "$pg\data\PG_VERSION")) { & "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust --encoding=UTF8 --locale=C }
& "$pg\bin\pg_isready.exe" -h localhost -p 5432
if ($LASTEXITCODE -ne 0) { & "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start }
ctest --test-dir server\build-release -C Release --output-on-failure
```

Для полного CTest и запуска сервера PostgreSQL должен быть доступен. Integration tests используют `fasc_test`; сам сервер готовит базу по конфигурации сборки:

```text
Debug:   fasc_test, очистка и повторная инициализация на каждом старте
Release: fasc_db, создание и инициализация при отсутствии схем
```

Основные runtime-настройки:

```text
FARM_DB_USER
FARM_DB_PASSWORD
FARM_DB_HOST
FARM_DB_PORT
FARM_DB_NAME
FARM_DB_BOOTSTRAP
FARM_DB_RESET_ON_START
FARM_SERVER_ADDRESS
FARM_SERVER_PORT
FARM_JWT_SECRET
FARM_ADMIN_ENABLED
FARM_ADMIN_LOGIN
FARM_ADMIN_PASSWORD
```

Основные маршруты:

```text
GET  /health
POST /auth/register   body: {"login":"alex","password":"password123"}
POST /auth/login      body: {"login":"alex","password":"password123"}
POST /users           Authorization: Bearer <manager-jwt>, body: {"login":"owner","password":"password123","role":"farm_owner"}
GET  /users           Authorization: Bearer <manager-jwt>
PUT  /users/role?login=owner  Authorization: Bearer <manager-jwt>, body: {"role":"agronomist"}
GET  /api/<resource>
POST /api/<resource>
GET  /api/<resource>/item?<key>
PUT  /api/<resource>/item?<key>
DELETE /api/<resource>/item?<key>
```

Auth-ответ содержит `token`, `token_type` и пользователя в форме `{ "login": "...", "role": "..." }`. Регистрация через `/auth/register` выдает роль `farm_worker`; `/users` предназначен для просмотра пользователей, создания пользователя с явной ролью и изменения ролей. Эти маршруты требуют JWT роли `agriculture_admin` или `association_director`.

При старте сервер создает встроенного администратора `admin` / `admin12345` с ролью `agriculture_admin`, если `FARM_ADMIN_ENABLED` не выключен. Логин и пароль переопределяются через `FARM_ADMIN_LOGIN` и `FARM_ADMIN_PASSWORD`.

Подробное руководство находится в корневом [README.md](../README.md) и в `docs/02_BUILD_RUN_TEST.md`.
