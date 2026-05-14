# 02. Сборка, запуск и тесты

Документ объясняет полный путь от чистого репозитория до работающего сервера и пройденных тестов.

## Структура сборки

Исходники лежат в:

```text
server/include
server/src
server/tests
```

Build directory создается отдельно:

```text
server/build
```

Это важно: исходники и результаты сборки не смешиваются. Каталог `server/build` можно удалить и создать заново без потери кода.

## Конфигурация CMake

Windows:

```powershell
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

Manjaro:

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

Смысл аргументов:

```text
cmake
  запускает этап конфигурации

-S server
  source directory; CMakeLists.txt находится в server

-B server/build
  build directory; сюда CMake положит Ninja-файлы, object files и generated files

-G Ninja
  выбрать генератор Ninja

-DCMAKE_CXX_COMPILER=clang++
  явно выбрать C++ компилятор

-DCMAKE_BUILD_TYPE=Debug
  собрать Debug-конфигурацию для single-config генератора Ninja
```

Синтаксис `-DNAME=value` задает CMake cache variable. CMake запоминает ее в `server/build/CMakeCache.txt`.

## Сборка

Windows:

```powershell
cmake --build server\build
```

Manjaro:

```bash
cmake --build server/build
```

Синтаксис:

```text
cmake --build <build-dir>
  не конфигурирует проект заново, а запускает выбранную build-систему
```

Так как генератор `Ninja`, фактически CMake вызывает Ninja с правильными путями.

## Что происходит во время сборки

Основные этапы:

```text
1. ODB compiler читает server/include/persistence/User.hpp
2. ODB генерирует user-odb.cxx, user-odb.hxx, user-odb.ixx и user.sql
3. C++ компилятор собирает farm_association_server_core
4. C++ компилятор собирает farm_association_server
5. C++ компилятор собирает GoogleTest targets
6. На Windows копируются PostgreSQL runtime DLL рядом с exe
```

`farm_association_server_core` - это библиотека с основной логикой. Она нужна, чтобы тесты могли линковаться к серверному коду без `main()`.

`farm_association_server` - это executable, который содержит `main.cpp`.

## Основные CMake options

```bash
-DFARM_SERVER_WITH_ODB=ON
```

Включает ODB/PostgreSQL слой. Это нормальный режим.

```bash
-DFARM_SERVER_BUILD_LOCAL_ODB_DEPS=ON
```

Разрешает CMake собрать локальные `libodb` и `libodb-pgsql`, если они еще не собраны.

```bash
-DFARM_SERVER_BUILD_TESTS=ON
```

Включает сборку GoogleTest unit/integration tests.

Пример отключения тестов:

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug -DFARM_SERVER_BUILD_TESTS=OFF
```

## PostgreSQL

Сервер берет параметры подключения из окружения:

```text
FARM_DB_USER      postgres
FARM_DB_PASSWORD  password
FARM_DB_NAME      farm_association
FARM_DB_HOST      localhost
FARM_DB_PORT      5432
```

Если переменная не задана, используется значение по умолчанию.

Windows пример:

```powershell
$env:FARM_DB_USER = "postgres"
$env:FARM_DB_PASSWORD = "password"
$env:FARM_DB_NAME = "farm_association"
$env:FARM_DB_HOST = "localhost"
$env:FARM_DB_PORT = "5432"
```

Manjaro пример:

```bash
export FARM_DB_USER=postgres
export FARM_DB_PASSWORD=password
export FARM_DB_NAME=farm_association
export FARM_DB_HOST=localhost
export FARM_DB_PORT=5432
```

## Применение SQL-схемы

ODB генерирует схему:

```text
server/build/generated/persistence/user.sql
```

В проекте есть и SQL-схема предметной области:

```text
database/dump-mydb-sprinthost-public.sql
```

Применение на Manjaro:

```bash
psql -h localhost -p 5432 -U postgres -d farm_association -f server/build/generated/persistence/user.sql
```

```bash
psql -h localhost -p 5432 -U postgres -d farm_association -f database/dump-mydb-sprinthost-public.sql
```

Применение на Windows через локальный PostgreSQL из `third_party`:

```powershell
$pg = "$PWD\server\third_party\postgresql"
& "$pg\bin\psql.exe" -h localhost -p 5432 -U postgres -d farm_association -f database\dump-mydb-sprinthost-public.sql
& "$pg\bin\psql.exe" -h localhost -p 5432 -U postgres -d farm_association -f server\build\generated\persistence\user.sql
```

Синтаксис `psql`:

```text
-h localhost
  host PostgreSQL

-p 5432
  port PostgreSQL

-U postgres
  user

-d farm_association
  database name

-f file.sql
  выполнить SQL из файла
```

## Запуск сервера

Windows:

```powershell
.\server\build\farm_association_server.exe
```

Manjaro:

```bash
./server/build/farm_association_server
```

Сервер регистрирует маршруты:

```text
GET  /health
POST /auth/register
POST /auth/login
POST /users
CRUD /api/*
```

И слушает:

```text
0.0.0.0:8080
```

## Проверка endpoints

Health:

```bash
curl http://localhost:8080/health
```

Создание пользователя:

```bash
curl -X POST http://localhost:8080/users -H "Content-Type: application/json" -d '{"name":"Alex"}'
```

Смысл аргументов `curl`:

```text
-X POST
  выбрать HTTP method POST

-H "Content-Type: application/json"
  добавить HTTP header

-d '{"name":"Alex"}'
  отправить request body
```

## Тесты

Команда:

```bash
ctest --test-dir server/build --output-on-failure
```

Windows:

```powershell
ctest --test-dir server\build -C Debug --output-on-failure
```

Синтаксис:

```text
ctest
  test runner из CMake

--test-dir server/build
  где искать CTestTestfile.cmake

-C Debug
  конфигурация для multi-config генераторов; для Ninja обычно не критично

--output-on-failure
  показать stdout/stderr теста, если он упал
```

Тестовый слой:

```text
unit/controller
unit/marshalling
integration/endpoints
integration/business
fixtures
utils
```

Unit tests проверяют маленькие части логики. Integration tests проверяют цепочки, например:

```text
BeastRequest -> RequestDispatcher -> AppRouter -> Handler -> BeastResponse
```
## Полный сценарий с нуля

Если репозиторий только что склонирован, порядок действий такой:

```text
1. установить системные инструменты
2. выполнить bootstrap зависимостей
3. выполнить cmake configure
4. выполнить cmake build
5. инициализировать PostgreSQL
6. создать базу farm_association
7. применить generated SQL-схему
8. запустить сервер
9. проверить endpoints
10. запустить тесты
```

Не стоит начинать с запуска `farm_association_server`, если база еще не создана. Программа создаст объект подключения к PostgreSQL, а первый persistence-сценарий упрется в отсутствие базы или таблицы.

## Configure, build и test

У CMake-проекта есть три разных этапа:

```text
configure
  CMake читает CMakeLists.txt, ищет зависимости, создает build files

build
  Ninja/компилятор собирают исходники в executable и библиотеки

test
  CTest запускает уже собранные test executables
```

Команды:

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build
ctest --test-dir server/build --output-on-failure
```

Если поменялся только `.cpp` или `.hpp`, обычно достаточно:

```bash
cmake --build server/build
```

Если поменялся `CMakeLists.txt`, лучше снова выполнить configure, а потом build.

## Как читать вывод сборки

Ninja печатает строки вида:

```text
[1/17] Generating ODB mapping for persistence/User.hpp
[2/17] Building CXX object ...
[17/17] Linking CXX executable farm_association_server.exe
```

Расшифровка:

```text
[1/17]
  номер текущего шага и общее количество шагов

Generating
  выполняется custom command, например ODB compiler

Building CXX object
  компилируется .cpp в object file

Linking CXX executable
  linker собирает final executable
```

Компиляция и линковка отличаются:

```text
compile error
  ошибка в C++ коде, include, типах, синтаксисе

link error
  object files собраны, но linker не нашел символ или библиотеку
```

Пример compile error:

```text
fatal error: 'controllers/app/Users.hpp' file not found
```

Означает, что include path или имя файла не совпадают.

Пример link error:

```text
unresolved external symbol
```

Означает, что объявление функции видно, но реализации нет в linked objects/libraries.

## Debug и Release

Debug:

```bash
-DCMAKE_BUILD_TYPE=Debug
```

Обычно включает отладочную информацию и отключает агрессивные оптимизации. Такой режим удобен для разработки.

Release:

```bash
-DCMAKE_BUILD_TYPE=Release
```

Обычно включает оптимизации. Такой режим нужен для измерения производительности и финальной сборки.

Для Ninja `CMAKE_BUILD_TYPE` задается на configure stage. Если нужно переключить Debug на Release, лучше создать отдельный build directory:

```bash
cmake -S server -B server/build-release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build server/build-release
```

## PostgreSQL на Windows подробнее

Если используется PostgreSQL из `server/third_party/postgresql`, можно создать локальный dev-кластер:

```powershell
$pg = "$PWD\server\third_party\postgresql"
& "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust
```

Синтаксис:

```text
$pg = "..."
  создать PowerShell переменную

&
  call operator; запускает путь, который хранится в строке

initdb.exe
  создает новый PostgreSQL data directory

-D "$pg\data"
  путь, где хранить database cluster

-U postgres
  имя superuser

-A trust
  local development auth без пароля
```

Запуск PostgreSQL:

```powershell
& "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start
```

Создание базы:

```powershell
& "$pg\bin\createdb.exe" -h localhost -p 5432 -U postgres farm_association
```

Если база уже существует, `createdb` сообщит об ошибке. Это не значит, что сервер сломан.

## PostgreSQL на Manjaro подробнее

Обычно PostgreSQL работает как systemd service:

```bash
sudo -iu postgres initdb -D /var/lib/postgres/data
sudo systemctl enable --now postgresql
```

Синтаксис:

```text
sudo -iu postgres
  выполнить команду от имени системного пользователя postgres

systemctl enable --now postgresql
  включить автозапуск сервиса и сразу запустить его
```

Создание пароля и базы:

```bash
sudo -iu postgres psql -c "ALTER USER postgres WITH PASSWORD 'password';"
sudo -iu postgres createdb farm_association
```

`psql -c` выполняет одну SQL-команду и завершает работу.

## Проверка работы базы

Проверить подключение:

```bash
psql -h localhost -p 5432 -U postgres -d farm_association -c "select 1;"
```

Проверить таблицы:

```bash
psql -h localhost -p 5432 -U postgres -d farm_association -c "\dt"
```

Если таблицы нет, применить схему:

```bash
psql -h localhost -p 5432 -U postgres -d farm_association -f server/build/generated/persistence/user.sql
```

## Диагностика запуска сервера

### Порт занят

Симптом:

```text
address already in use
```

Windows:

```powershell
netstat -ano | findstr :8080
```

Manjaro:

```bash
ss -ltnp | grep :8080
```

### Ошибка подключения к PostgreSQL

Проверь:

```text
1. PostgreSQL запущен
2. база farm_association создана
3. FARM_DB_* переменные совпадают с реальными настройками
4. SQL-схема применена
```

### `POST /users` возвращает 500

Router превращает исключения handler-а в HTTP 500:

```json
{"error":"..."}
```

Если body:

```json
{"name":""}
```

то бизнес-логика вернет:

```text
User name is required
```

Если body невалидный JSON, ошибку выбросит `nlohmann::json`.

## Как добавлять новый endpoint

Путь по слоям:

```text
1. DTO или command в controllers/dto
2. View в views
3. from_json/to_json в marshalling
4. application method в controllers/app
5. HTTP method в controllers/http
6. handler в handling
7. route registration в main.cpp
8. unit tests
9. integration endpoint/business tests
```

Для `GET /users/{id}` нужно будет решить, как router поддерживает path parameters. Сейчас `AppRouter` ищет точное совпадение строки `METHOD path`, поэтому динамические пути пока не поддерживаются.

## Как понимать результаты CTest

Пример:

```text
1/13 Test #1: farm_association_server_version ... Passed
10/13 Test #10: integration.UsersBusinessProcessTests... Passed
```

Название теста состоит из:

```text
prefix от gtest_discover_tests
test suite
test case
```

Например:

```text
integration.UsersBusinessProcessTests.PostUsersWithEmptyNameReturnsBusinessErrorThroughHttpPipeline
```

Расшифровка:

```text
integration
  тест относится к integration target

UsersBusinessProcessTests
  группа тестов

PostUsersWithEmptyNameReturnsBusinessErrorThroughHttpPipeline
  конкретный сценарий
```
