# 02. Сборка, Запуск И Тесты

Документ описывает реальный рабочий цикл проекта: подготовка зависимостей, Debug/Release сборка, запуск сервера и проверка тестов.

## Каталоги Сборки

Исходники лежат в `server/include`, `server/src` и `server/tests`. Результаты сборки держим отдельно:

```text
server/build          Debug
server/build-release  Release
```

Если нужно начать заново, можно удалить только build-каталог. Исходный код и зависимости в `server/third_party` при этом не затрагиваются.

## Debug

Windows:

```powershell
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server\build --config Debug --parallel
$pg = "$PWD\server\third_party\postgresql"
if (!(Test-Path "$pg\data\PG_VERSION")) { & "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust --encoding=UTF8 --locale=C }
& "$pg\bin\pg_isready.exe" -h localhost -p 5432
if ($LASTEXITCODE -ne 0) { & "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start }
ctest --test-dir server\build -C Debug --output-on-failure
```

Manjaro:

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build --config Debug --parallel
sudo test -f /var/lib/postgres/data/PG_VERSION || sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"
ctest --test-dir server/build -C Debug --output-on-failure
```

Debug-сервер при старте использует базу `fasc_test`, каждый раз удаляет ее, создает заново и применяет SQL-инициализацию. Это удобно для разработки и ручной проверки: состояние базы не протухает между запусками.

## Release

Windows:

```powershell
cmake -S server -B server\build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build server\build-release --config Release --parallel
$pg = "$PWD\server\third_party\postgresql"
if (!(Test-Path "$pg\data\PG_VERSION")) { & "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust --encoding=UTF8 --locale=C }
& "$pg\bin\pg_isready.exe" -h localhost -p 5432
if ($LASTEXITCODE -ne 0) { & "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start }
ctest --test-dir server\build-release -C Release --output-on-failure
```

Manjaro:

```bash
cmake -S server -B server/build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build server/build-release --config Release --parallel
sudo test -f /var/lib/postgres/data/PG_VERSION || sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"
ctest --test-dir server/build-release -C Release --output-on-failure
```

Release-сервер при старте использует базу `fasc_db`. Если базы нет, он создает ее. Если отсутствуют основные таблицы, он применяет SQL-инициализацию. Существующие данные в Release не очищаются.

## Что Делает Build

Основные шаги:

```text
1. ODB compiler читает server/include/persistence/User.hpp
2. Генерирует user-odb.cxx, user-odb.hxx, user-odb.ixx и user.sql
3. При необходимости собирает локальные libodb/libodb-pgsql для текущей конфигурации
4. Собирает farm_association_server_core
5. Собирает farm_association_server
6. Собирает GoogleTest targets
7. Копирует SQL bootstrap scripts рядом с executable в каталог sql/
8. На Windows копирует PostgreSQL runtime DLL рядом с executable и tests
```

`farm_association_server_core` содержит основную серверную логику без `main()`, чтобы тесты могли линковаться к тому же коду. `farm_association_server` содержит только точку входа и запускает HTTP-сервер.

## PostgreSQL

Unit tests не требуют запущенный PostgreSQL. Полный CTest включает DB-backed integration tests, поэтому перед запуском `ctest` PostgreSQL должен быть доступен на `FARM_DB_HOST:FARM_DB_PORT`.

DB-backed integration tests всегда используют `fasc_test`, даже если проверяется Release build. Перед DB-backed проверкой тесты сбрасывают `fasc_test`, применяют предметный SQL и generated ODB `user.sql`, затем проверяют чтение `/api/farm` и auth register/login через реальную БД.

Для запуска сервера PostgreSQL должен быть доступен на `FARM_DB_HOST:FARM_DB_PORT`. Базу и схемы сервер готовит сам.

Переменные подключения:

```text
FARM_DB_USER              postgres
FARM_DB_PASSWORD          password
FARM_DB_HOST              localhost
FARM_DB_PORT              5432
FARM_DB_NAME              fasc_test в Debug, fasc_db в Release
FARM_DB_MAINTENANCE_NAME  postgres
```

Переменные bootstrap:

```text
FARM_DB_BOOTSTRAP
  true/false, 1/0, yes/no, on/off; по умолчанию включен

FARM_DB_RESET_ON_START
  по умолчанию true в Debug и false в Release

FARM_DB_SCHEMA_SQL
  путь к SQL предметной области; по умолчанию database/dump-mydb-sprinthost-public.sql

FARM_DB_USER_SQL
  путь к generated ODB SQL для User
```

Ручные `createdb` и `psql -f ...` теперь не являются частью обычного сценария запуска. Они могут пригодиться только для диагностики.

## Запуск PostgreSQL

Windows, локальный PostgreSQL из `server/third_party`:

```powershell
$pg = "$PWD\server\third_party\postgresql"
& "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust
& "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start
& "$pg\bin\pg_isready.exe" -h localhost -p 5432
```

Если кластер уже создан, `initdb` повторно запускать не нужно:

```powershell
$pg = "$PWD\server\third_party\postgresql"
& "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start
```

Manjaro:

```bash
sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"
pg_isready -h localhost -p 5432
```

## Запуск Сервера

Debug Windows:

```powershell
.\server\build\farm_association_server.exe
```

Debug Manjaro:

```bash
./server/build/farm_association_server
```

Release:

```bash
./server/build-release/farm_association_server
```

По умолчанию сервер слушает `0.0.0.0:8080`.

HTTP-настройки:

```text
FARM_SERVER_ADDRESS
FARM_SERVER_PORT
FARM_SERVER_THREADS
FARM_SERVER_BODY_LIMIT
FARM_SERVER_TIMEOUT_SECONDS
FARM_SERVER_LISTEN_BACKLOG
FARM_JWT_SECRET
FARM_ADMIN_ENABLED
FARM_ADMIN_LOGIN
FARM_ADMIN_PASSWORD
```

## Проверка Endpoints

Health:

```bash
curl http://localhost:8080/health
```

Регистрация и логин:

```bash
curl -X POST http://localhost:8080/auth/register -H "Content-Type: application/json" -d '{"login":"alex","password":"password123"}'
curl -X POST http://localhost:8080/auth/login -H "Content-Type: application/json" -d '{"login":"alex","password":"password123"}'
curl -X POST http://localhost:8080/auth/login -H "Content-Type: application/json" -d '{"login":"admin","password":"admin12345"}'
curl -X POST http://localhost:8080/users -H "Content-Type: application/json" -H "Authorization: Bearer <admin-jwt>" -d '{"login":"owner","password":"password123","role":"farm_owner"}'
```

`/auth/register` создает пользователя с ролью `farm_worker`. Сервер при старте создает встроенного администратора `admin` / `admin12345` с ролью `agriculture_admin`; значения задаются через `FARM_ADMIN_LOGIN` и `FARM_ADMIN_PASSWORD`, создание отключается через `FARM_ADMIN_ENABLED=0`. Успешный ответ `/auth/register` и `/auth/login` содержит `token`, `token_type` и `user.login`/`user.role`. `POST /users` принимает явную роль только с JWT администратора.

Пример чтения предметной сущности:

```bash
curl http://localhost:8080/api/farm
curl http://localhost:8080/api/farm/item?id=1
curl -X PUT "http://localhost:8080/api/farm/item?id=1" -H "Content-Type: application/json" -d '{"status":"active"}'
```

Все `/api/*` endpoints ожидают и возвращают JSON с типами view-слоя, а не persistence-структуры. Схема маршрутов: `GET/POST /api/<resource>` и `GET/PUT/DELETE /api/<resource>/item?<key>`.

## Тесты

Тесты запускаются через CTest:

```powershell
ctest --test-dir server\build -C Debug --output-on-failure
```

```bash
ctest --test-dir server/build -C Debug --output-on-failure
```

Текущий набор проверяет:

```text
unit/controller
unit/database
unit/marshalling
integration/database
integration/endpoints
integration/business
fixtures
utils
```

На текущей ревизии ожидается полное прохождение набора CTest при запущенном PostgreSQL. Последняя проверка Debug прошла `113/113` тестов. Если число тестов изменилось после добавления новых сценариев, ориентируйся на фактический вывод `ctest`.

## Диагностика

Порт занят:

```powershell
netstat -ano | findstr :8080
```

```bash
ss -ltnp | grep :8080
```

PostgreSQL недоступен:

```bash
pg_isready -h localhost -p 5432
```

Если сервер падает на старте из-за БД, проверь:

```text
1. PostgreSQL запущен
2. FARM_DB_USER/FARM_DB_PASSWORD совпадают с реальным пользователем
3. FARM_DB_HOST/FARM_DB_PORT указывают на доступный сервер PostgreSQL
4. FARM_DB_MAINTENANCE_NAME отличается от FARM_DB_NAME
5. SQL-файлы существуют, если FARM_DB_SCHEMA_SQL или FARM_DB_USER_SQL переопределены
```

Windows error `0xc0000135` обычно означает, что рядом с executable нет нужной DLL. Пересобери проект: CMake копирует PostgreSQL runtime DLL после build.
