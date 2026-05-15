# Farm Association

C++20 HTTP-сервер для системы фермерской ассоциации. Сервер использует Boost.Asio/Beast, nlohmann/json, fmt, ODB, PostgreSQL/libpq и OpenSSL. Поддерживаемый сценарий сборки: Windows и Manjaro через CMake + Ninja + clang++.

Внешние библиотеки не хранятся в Git. Каталог `server/third_party` является локальным состоянием машины разработчика и восстанавливается bootstrap-скриптами. В репозитории должен оставаться только `server/third_party/.gitkeep`.

## Быстрый Старт

Windows:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
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
sudo pacman -S --needed base-devel cmake ninja clang curl unzip tar openssl postgresql postgresql-libs
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build --parallel
sudo test -f /var/lib/postgres/data/PG_VERSION || sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"
ctest --test-dir server/build -C Debug --output-on-failure
```

Release лучше собирать в отдельный каталог:

```bash
cmake -S server -B server/build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build server/build-release --config Release --parallel
ctest --test-dir server/build-release -C Release --output-on-failure
```

## Зависимости

Bootstrap готовит структуру, которую ожидает CMake:

```text
server/third_party/boost/include/boost/version.hpp
server/third_party/nlohmann_json/include/nlohmann/json.hpp
server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
server/third_party/odb/bin/odb(.exe)
server/third_party/postgresql/include/libpq-fe.h
```

На Windows OpenSSL приходит вместе с локальным PostgreSQL:

```text
server/third_party/postgresql/include/openssl/evp.h
server/third_party/postgresql/lib/libcrypto.lib
```

На Manjaro bootstrap копирует системные OpenSSL/libpq файлы в локальное дерево:

```text
server/third_party/openssl/include/openssl/evp.h
server/third_party/openssl/lib/libcrypto.so
server/third_party/postgresql/lib/libpq.so
```

Локальные `libodb` и `libodb-pgsql` собираются CMake-ом из `server/third_party/_sources` при первом build. Для Windows они разделены по конфигурациям, чтобы Debug и Release не смешивали несовместимые runtime-библиотеки:

```text
server/third_party/libodb-Debug
server/third_party/libodb-pgsql-Debug
server/third_party/libodb-Release
server/third_party/libodb-pgsql-Release
```

Принудительно скачать зависимости заново:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1 -Force
```

```bash
FORCE=1 ./tools/bootstrap-deps.sh
```

## База Данных

Для сборки и unit-тестов запущенный PostgreSQL не нужен. Полный CTest включает DB-backed integration tests, поэтому перед `ctest` PostgreSQL должен быть доступен. Тесты всегда используют `fasc_test`; сервер при обычном запуске выбирает базу по конфигурации сборки.

Поведение по умолчанию:

```text
Debug
  database: fasc_test
  mode: drop/create на каждом старте сервера
  data: загружается предметная схема и ODB-схема пользователя

Release
  database: fasc_db
  mode: create/initialize when missing
  data: существующие данные не очищаются
```

Сервер использует параметры подключения:

```text
FARM_DB_USER              postgres
FARM_DB_PASSWORD          password
FARM_DB_HOST              localhost
FARM_DB_PORT              5432
FARM_DB_NAME              build default: fasc_test или fasc_db
FARM_DB_MAINTENANCE_NAME  postgres
```

Дополнительные настройки bootstrap:

```text
FARM_DB_BOOTSTRAP=0
  отключить подготовку БД на старте

FARM_DB_RESET_ON_START=1
  принудительно очистить и пересоздать целевую БД

FARM_DB_SCHEMA_SQL=<path>
  переопределить SQL предметной области

FARM_DB_USER_SQL=<path>
  переопределить SQL ODB-схемы User
```

Ручные `createdb` и `psql -f ...` для нормального запуска больше не нужны.

## Запуск

Windows:

```powershell
.\server\build\farm_association_server.exe
```

Manjaro:

```bash
./server/build/farm_association_server
```

По умолчанию сервер слушает `0.0.0.0:8080`. Параметры HTTP-сервера можно переопределить через `FARM_SERVER_ADDRESS`, `FARM_SERVER_PORT`, `FARM_SERVER_THREADS`, `FARM_SERVER_BODY_LIMIT`, `FARM_SERVER_TIMEOUT_SECONDS`, `FARM_SERVER_LISTEN_BACKLOG`.

Проверка после запуска:

```bash
curl http://localhost:8080/health
curl -X POST http://localhost:8080/auth/register -H "Content-Type: application/json" -d '{"name":"Alex","password":"password123"}'
curl http://localhost:8080/api/farm
```

## Документация

- `docs/01_DEPENDENCIES_SETUP.md` - установка зависимостей на Windows и Manjaro.
- `docs/02_BUILD_RUN_TEST.md` - сборка, запуск, PostgreSQL и тесты.
- `docs/03_CMAKE_GUIDE.md` - устройство CMake и targets.
- `docs/04_BOOST_ASIO_BEAST_SERVER.md` - HTTP-сервер на Boost.Asio/Beast.
- `docs/05_HIGH_PERFORMANCE_BEAST_SERVER.md` - заметки по высокопроизводительному Beast server.
- `server/docs/module-spec.md` - спецификация серверного модуля.

## Client

В каталоге `client` инициализирован React + TypeScript проект для role-based интерфейса фермерской ассоциации.

```powershell
cd client
npm install
npm run dev
```

По умолчанию Vite проксирует `/server-api/*` в C++ сервер `http://127.0.0.1:8080`. Переопределить backend можно через `FARM_SERVER_URL` или `VITE_FARM_SERVER_URL`.

```powershell
npm run typecheck
npm test
npm run build
```

Спецификация клиента: `client/SPECIFICATION.md`.

## Git Policy

Перед push проверь:

```bash
git ls-files server/third_party
```

Ожидаемый вывод:

```text
server/third_party/.gitkeep
```
