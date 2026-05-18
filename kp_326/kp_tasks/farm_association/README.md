# Farm Association

Проект системы фермерской ассоциации: C++20 HTTP-сервер и React + TypeScript клиент. Сервер использует Boost.Asio/Beast, nlohmann/json, fmt, ODB, PostgreSQL/libpq и OpenSSL. В dev-режиме Vite проксирует клиентские `/auth`, `/users`, `/api` и `/health` запросы в серверный JSON API; сервер также отдает CORS/preflight-заголовки. Поддерживаемый сценарий сборки сервера: Windows и Manjaro через CMake + Ninja + clang++.

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
sudo pacman -S --needed base-devel cmake ninja clang curl unzip tar openssl postgresql postgresql-libs nodejs npm git
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug -DFARM_SERVER_USE_CHECKED_IN_ODB=ON
cmake --build server/build --parallel
sudo test -f /var/lib/postgres/data/PG_VERSION || sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"
ctest --test-dir server/build --output-on-failure
```

## Полное Развертывание На Manjaro

Этот маршрут поднимает всю систему с нуля: зависимости, сервер, PostgreSQL, API smoke-проверку и клиент.

```bash
sudo pacman -S --needed base-devel cmake ninja clang curl unzip tar openssl postgresql postgresql-libs nodejs npm git
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh

cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug -DFARM_SERVER_USE_CHECKED_IN_ODB=ON
cmake --build server/build --parallel

sudo test -f /var/lib/postgres/data/PG_VERSION || sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"

ctest --test-dir server/build --output-on-failure
./server/build/farm_association_server &
SERVER_PID=$!
sleep 2

curl http://localhost:8080/health
ADMIN_TOKEN=$(curl -sS -X POST http://localhost:8080/auth/login -H "Content-Type: application/json" -d '{"login":"admin","password":"admin12345"}' | node -pe "JSON.parse(require('fs').readFileSync(0, 'utf8')).token")
curl -X POST http://localhost:8080/auth/register -H "Content-Type: application/json" -d '{"login":"alex","password":"password123"}'
curl -X POST http://localhost:8080/auth/login -H "Content-Type: application/json" -d '{"login":"alex","password":"password123"}'
curl -X POST http://localhost:8080/users -H "Content-Type: application/json" -H "Authorization: Bearer ${ADMIN_TOKEN}" -d '{"login":"owner","password":"password123","role":"farm_owner"}'
curl http://localhost:8080/users -H "Authorization: Bearer ${ADMIN_TOKEN}"
curl -X PUT "http://localhost:8080/users/role?login=owner" -H "Content-Type: application/json" -H "Authorization: Bearer ${ADMIN_TOKEN}" -d '{"role":"agronomist"}'
curl http://localhost:8080/api/farm

cd client
npm install
npm test -- --run
npm run typecheck
npm run build
cd ..
kill "$SERVER_PID"
```

В Docker/CI без systemd PostgreSQL можно запускать вручную: `runuser -u postgres -- initdb -D /tmp/farm_pgdata --encoding=UTF8 --locale=C`, затем `runuser -u postgres -- pg_ctl -D /tmp/farm_pgdata -l /tmp/farm_pg.log -o '-h 127.0.0.1 -p 5432 -k /tmp' start -w`. На обычной Manjaro-машине достаточно `systemctl enable --now postgresql`.

Release лучше собирать в отдельный каталог:

```bash
cmake -S server -B server/build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release -DFARM_SERVER_USE_CHECKED_IN_ODB=ON
cmake --build server/build-release --parallel
ctest --test-dir server/build-release --output-on-failure
```

## Зависимости

Bootstrap готовит структуру, которую ожидает CMake:

```text
server/third_party/boost/include/boost/version.hpp
server/third_party/nlohmann_json/include/nlohmann/json.hpp
server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
server/third_party/postgresql/include/libpq-fe.h
```

На Windows bootstrap также кладет `server/third_party/odb/bin/odb.exe`, и CMake может регенерировать ODB-код. На Manjaro скачивание ODB compiler по умолчанию пропускается: сборка использует проверенные `server/generated/persistence/*-odb.*`, чтобы не зависеть от медленного/нестабильного архива CodeSynthesis.

На Windows OpenSSL приходит вместе с локальным PostgreSQL:

```text
server/third_party/postgresql/include/openssl/evp.h
server/third_party/postgresql/lib/libcrypto.lib
```

На Manjaro CMake использует системные OpenSSL/libpq из пакетов `openssl` и `postgresql-libs`; bootstrap также может подготовить локальную копию OpenSSL/libpq, но Linux-сборка не подмешивает Windows-бандл PostgreSQL в include path:

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

Сервер открывает libpq/ODB-подключения с `client_encoding=UTF8`. Новые базы, которые создает bootstrap, создаются как UTF-8 (`template0`, `LC_COLLATE=C`, `LC_CTYPE=C`), а для уже существующих баз PostgreSQL выполняет конвертацию результата в UTF-8 на уровне клиента. Это важно на Windows-кластерах с русской локалью `WIN1251`: JSON API остается валидным UTF-8, и `/api/farm` не падает на русских сидовых данных.

Предметные таблицы и пользователи имеют ODB mapping в `server/include/persistence` и generated-файлы в `server/generated/persistence`. CRUD facade `Database::selectRows/insertRow/updateRows/deleteRows` для `/api/*` теперь dispatch-ит в ODB registry, а прямой libpq SQL остается для bootstrap и низкоуровневой диагностики.

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

Сборка и запуск сервера на Manjaro, если зависимости и PostgreSQL уже подготовлены:

```bash
sudo systemctl enable --now postgresql
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build --parallel
./server/build/farm_association_server
```

По умолчанию сервер слушает `0.0.0.0:8080`. Параметры HTTP-сервера можно переопределить через `FARM_SERVER_ADDRESS`, `FARM_SERVER_PORT`, `FARM_SERVER_THREADS`, `FARM_SERVER_BODY_LIMIT`, `FARM_SERVER_TIMEOUT_SECONDS`, `FARM_SERVER_LISTEN_BACKLOG`.

При старте сервер создает встроенного администратора, если он еще не существует:

```text
login:    admin
password: admin12345
role:     agriculture_admin
```

Учетку можно отключить или переопределить через `FARM_ADMIN_ENABLED`, `FARM_ADMIN_LOGIN` и `FARM_ADMIN_PASSWORD`. Пароль должен содержать минимум 8 символов.

Проверка после запуска:

```bash
curl http://localhost:8080/health
curl -X POST http://localhost:8080/auth/login -H "Content-Type: application/json" -d '{"login":"admin","password":"admin12345"}'
curl -X POST http://localhost:8080/auth/register -H "Content-Type: application/json" -d '{"login":"alex","password":"password123"}'
curl -X POST http://localhost:8080/auth/login -H "Content-Type: application/json" -d '{"login":"alex","password":"password123"}'
curl -X POST http://localhost:8080/users -H "Content-Type: application/json" -H "Authorization: Bearer <admin-or-director-jwt>" -d '{"login":"owner","password":"password123","role":"farm_owner"}'
curl http://localhost:8080/users -H "Authorization: Bearer <admin-or-director-jwt>"
curl -X PUT "http://localhost:8080/users/role?login=owner" -H "Content-Type: application/json" -H "Authorization: Bearer <admin-or-director-jwt>" -d '{"role":"agronomist"}'
curl http://localhost:8080/api/farm
```

Основные пользовательские маршруты: `POST /auth/register`, `POST /auth/login`, `POST /users`, `GET /users`, `PUT /users/role?login=<login>`. Регистрация через `/auth/register` создает пользователя с ролью `farm_worker`; управленческие маршруты `/users` принимают/возвращают поле `role` и требуют `Authorization: Bearer <token>` пользователя с ролью `agriculture_admin` или `association_director`. Успешная авторизация возвращает JWT и пользователя в виде `{ "login": "...", "role": "..." }`.

Роль пользователя хранится на сервере в таблице пользователей, возвращается в auth-ответе и записывается в JWT claim `role`. Клиент использует это значение для показа разделов; сервер уже проверяет роль на маршрутах управления пользователями. Сейчас управленческими считаются `agriculture_admin` и `association_director`.

## Документация

- `docs/00_DOCUMENTATION_MAP.md` - полный список документов проекта и маршруты чтения под разные задачи.
- `docs/01_DEPENDENCIES_SETUP.md` - установка зависимостей на Windows и Manjaro.
- `docs/02_BUILD_RUN_TEST.md` - сборка, запуск, PostgreSQL и тесты.
- `docs/03_CMAKE_GUIDE.md` - устройство CMake и targets.
- `docs/04_BOOST_ASIO_BEAST_SERVER.md` - HTTP-сервер на Boost.Asio/Beast.
- `docs/05_HIGH_PERFORMANCE_BEAST_SERVER.md` - заметки по высокопроизводительному Beast server.
- `docs/06_SYSTEM_AND_SYNTAX_GUIDE.md` - как работает сервер, клиент, API, роли, таблицы, а также используемый синтаксис backend/frontend.
- `server/docs/module-spec.md` - спецификация серверного модуля.

## Client

В каталоге `client` находится React + TypeScript проект для role-based интерфейса фермерской ассоциации. Клиент показывает доступные бизнес-модули по роли из серверного auth-ответа, дает `agriculture_admin` и `association_director` экран просмотра пользователей, создания учетных записей и изменения ролей, загружает таблицы через `GET /api/<resource>`, фильтрует и ищет строки на клиенте, создает записи через `POST /api/<resource>`, обновляет и удаляет записи через `/api/<resource>/item?<key>`.

```powershell
cd client
npm install
npm run dev
```

По умолчанию dev-прокси Vite отправляет API-запросы в C++ сервер `http://127.0.0.1:8080`. Для другого порта сервера перед `npm run dev` задай `FARM_SERVER_URL`, например `$env:FARM_SERVER_URL = "http://127.0.0.1:18081"`. Если нужен прямой backend URL без Vite proxy, можно собрать клиент с `VITE_API_BASE_URL`.

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
