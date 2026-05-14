# Farm Association

C++20 HTTP-сервер для системы фермерской ассоциации. Сервер использует Boost.Asio/Beast, nlohmann/json, fmt, ODB, PostgreSQL/libpq и OpenSSL. Сборка поддерживается на Windows и Manjaro через CMake + Ninja + clang++.

Проект не хранит внешние библиотеки в Git. Все локальные зависимости восстанавливаются bootstrap-скриптами в `server/third_party`; в репозитории должен оставаться только `server/third_party/.gitkeep`.

## Быстрый Старт

Windows:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server\build
ctest --test-dir server\build -C Debug --output-on-failure
```

Manjaro:

```bash
sudo pacman -S --needed base-devel cmake ninja clang curl unzip tar openssl postgresql postgresql-libs
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build
ctest --test-dir server/build --output-on-failure
```

На текущей машине Windows этот цикл проверен: configure, build и `56/56` CTest проходят.

## Что Скачивает Bootstrap

`tools/bootstrap-deps.ps1` и `tools/bootstrap-deps.sh` готовят структуру, которую ожидает CMake:

```text
server/third_party/boost/include/boost/version.hpp
server/third_party/nlohmann_json/include/nlohmann/json.hpp
server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
server/third_party/odb/bin/odb(.exe)
server/third_party/postgresql/include/libpq-fe.h
server/third_party/postgresql/lib/libpq.*
```

На Windows PostgreSQL binaries также дают OpenSSL headers/import library:

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

Если нужно принудительно скачать зависимости заново:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1 -Force
```

```bash
FORCE=1 ./tools/bootstrap-deps.sh
```

## Сборка

Основной поддерживаемый вариант:

```text
CMake + Ninja + clang++
```

CMake options, которые реально используются:

```text
FARM_SERVER_BUILD_TESTS=ON
  собирать GoogleTest unit/integration tests

FARM_SERVER_BUILD_LOCAL_ODB_DEPS=ON
  собрать libodb и libodb-pgsql из server/third_party/_sources при первом build

FARM_SERVER_ODB_STATIC=ON
  линковать локальные ODB runtime-библиотеки как static libraries
```

`FARM_SERVER_WITH_ODB` оставлен включенным. Текущий сервер использует ODB/PostgreSQL в основном коде, поэтому штатная сборка проекта предполагает ODB.

Release-сборку лучше делать в отдельный каталог:

```bash
cmake -S server -B server/build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build server/build-release
ctest --test-dir server/build-release --output-on-failure
```

## База Данных

Для компиляции и текущих тестов запущенный PostgreSQL не нужен. Для запуска сервера и CRUD endpoints нужна база `farm_association`.

Сервер читает параметры подключения из окружения:

```text
FARM_DB_USER      postgres
FARM_DB_PASSWORD  password
FARM_DB_NAME      farm_association
FARM_DB_HOST      localhost
FARM_DB_PORT      5432
```

Если переменные не заданы, используются значения выше.

В проекте есть две SQL-части:

```text
database/dump-mydb-sprinthost-public.sql
  схема и данные предметной области: farm, person, product, association_* и т.д.

server/build/generated/persistence/user.sql
  ODB-схема таблицы "User" для auth/users
```

Windows с локальным PostgreSQL из `server/third_party`:

```powershell
$pg = "$PWD\server\third_party\postgresql"
& "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust
& "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start
& "$pg\bin\createdb.exe" -h localhost -p 5432 -U postgres farm_association
& "$pg\bin\psql.exe" -h localhost -p 5432 -U postgres -d farm_association -f database\dump-mydb-sprinthost-public.sql
& "$pg\bin\psql.exe" -h localhost -p 5432 -U postgres -d farm_association -f server\build\generated\persistence\user.sql
```

Manjaro:

```bash
sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"
sudo -iu postgres createdb farm_association
psql -h localhost -p 5432 -U postgres -d farm_association -f database/dump-mydb-sprinthost-public.sql
psql -h localhost -p 5432 -U postgres -d farm_association -f server/build/generated/persistence/user.sql
```

Если база уже создана, `createdb` может сообщить об ошибке. Это нормально; продолжай с применением SQL-файлов.

## Запуск

Windows:

```powershell
.\server\build\farm_association_server.exe
```

Manjaro:

```bash
./server/build/farm_association_server
```

Сервер слушает `0.0.0.0:8080`.

Проверка:

```bash
curl http://localhost:8080/health
curl -X POST http://localhost:8080/users -H "Content-Type: application/json" -d '{"name":"Alex","password":"password123"}'
curl http://localhost:8080/api/farm
```

## Тесты

Unit и integration tests запускаются через CTest:

```powershell
ctest --test-dir server\build -C Debug --output-on-failure
```

```bash
ctest --test-dir server/build --output-on-failure
```

Текущий набор тестов проверяет server version, роутер, health/users pipeline, security, JSON marshalling и регистрацию farm entity routes.

## Частые Проблемы

`Missing local dependency path`
: bootstrap не был выполнен или завершился неуспешно. Запусти `tools/bootstrap-deps.*` заново.

`openssl/evp.h` или `libcrypto` не найдены на Manjaro
: установи `openssl` и повтори `FORCE=1 ./tools/bootstrap-deps.sh`.

`libpq-fe.h` или `libpq` не найдены на Manjaro
: установи `postgresql-libs` и повтори bootstrap.

`odb` не найден
: проверь `server/third_party/odb/bin/odb` или `server/third_party/odb/bin/odb.exe`.

Windows error `0xc0000135`
: рядом с executable нет нужных DLL. После `cmake --build` CMake копирует PostgreSQL DLL в `server/build` и `server/build/tests`; если каталог очищался вручную, пересобери проект.

## Документация

- `docs/01_DEPENDENCIES_SETUP.md` - подробная установка зависимостей.
- `docs/02_BUILD_RUN_TEST.md` - сборка, запуск, PostgreSQL и тесты.
- `docs/03_CMAKE_GUIDE.md` - устройство CMake.
- `docs/04_BOOST_ASIO_BEAST_SERVER.md` - HTTP-сервер на Boost.Asio/Beast.
- `docs/05_HIGH_PERFORMANCE_BEAST_SERVER.md` - заметки по высокопроизводительному Beast server.

## Git Policy

Проверка перед push:

```bash
git ls-files server/third_party
```

Ожидаемый вывод:

```text
server/third_party/.gitkeep
```
