# 03. CMake Guide

Документ описывает, как устроен `server/CMakeLists.txt` и почему проект собирается одинаково на Windows и Manjaro.

## Базовая Схема

Проект использует CMake как генератор build-системы:

```text
server/CMakeLists.txt -> Ninja files -> clang++ -> executable/tests
```

Основная команда configure:

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

Основная команда build:

```bash
cmake --build server/build --config Debug --parallel
```

Для Ninja `CMAKE_BUILD_TYPE` задается на этапе configure. Поэтому Debug и Release лучше держать в разных каталогах:

```text
server/build
server/build-release
```

## Options

Поддерживаемые CMake options:

```cmake
option(FARM_SERVER_WITH_ODB "Build with ODB PostgreSQL support" ON)
option(FARM_SERVER_ODB_STATIC "Link ODB runtime as static libraries" ON)
option(FARM_SERVER_BUILD_LOCAL_ODB_DEPS "Build local libodb/libodb-pgsql archives" ON)
option(FARM_SERVER_BUILD_TESTS "Build unit and integration tests" ON)
```

Обычная сборка оставляет все options включенными. `FARM_SERVER_BUILD_LOCAL_ODB_DEPS=ON` означает, что CMake сам соберет локальные `libodb` и `libodb-pgsql` из `server/third_party/_sources`.

## Targets

Основные targets:

```text
farm_association_server_core
farm_association_server
farm_association_unit_tests
farm_association_integration_tests
```

`farm_association_server_core` - статическая библиотека с основной логикой: controllers, handlers, database layer, security, router, dispatcher и server core.

`farm_association_server` - executable с `src/Main.cpp`.

Tests линкуются к `farm_association_server_core`, чтобы проверять тот же код, который использует сервер, но без второго `main()`.

## Sources

Ключевой список source-файлов находится в `FARM_SERVER_CORE_SOURCES`:

```cmake
set(FARM_SERVER_CORE_SOURCES
        src/controllers/app/Users.cpp
        src/controllers/http/Users.cpp
        src/database/Bootstrap.cpp
        src/database/Database.cpp
        src/handling/Health.cpp
        src/handling/Users.cpp
        ${FARM_SERVER_ENTITY_SOURCES}
        src/security/Encoding.cpp
        src/security/JwtService.cpp
        src/security/PasswordHasher.cpp
        src/server/core/AppRouter.cpp
        src/server/core/RequestDispatcher.cpp
        src/server/core/Server.cpp
        "${FARM_SERVER_USER_ODB_CXX}")
```

`FARM_SERVER_ENTITY_SOURCES` собирает app/http controllers и handlers для предметных сущностей через `file(GLOB ... CONFIGURE_DEPENDS)`. Специальные `Users.cpp` и `Health.cpp` подключаются явно, потому что у них отдельная логика.

## Generated ODB Files

ODB читает:

```text
server/include/persistence/User.hpp
```

На текущей ревизии ODB generation включен только для пользовательской модели. Предметные CRUD-сущности из SQL-дампа имеют persistence-структуры в `server/include/persistence`, но работают через универсальные SQL-методы `Database` без отдельных generated `*-odb.*` файлов.

И генерирует:

```text
server/build/generated/persistence/user-odb.cxx
server/build/generated/persistence/user-odb.hxx
server/build/generated/persistence/user-odb.ixx
server/build/generated/persistence/user.sql
```

Генерация описана через `add_custom_command`. Generated header подключается через include path `server/build/generated`, поэтому код может писать:

```cpp
#include <persistence/user-odb.hxx>
```

## Локальные ODB Runtime Libraries

Сборка ODB runtime вынесена в `server/cmake/FarmLocalDependencies.cmake` и `server/cmake/odb-local/CMakeLists.txt`.

На Windows `libodb` и `libodb-pgsql` собираются в конфигурационные каталоги:

```text
server/third_party/libodb-Debug
server/third_party/libodb-pgsql-Debug
server/third_party/libodb-Release
server/third_party/libodb-pgsql-Release
```

Это защищает от смешивания Debug и Release runtime-библиотек. На Linux CMake использует локальные библиотеки из того же механизма, но без ручной установки ODB runtime в систему.

## Compile Definitions

Сервер получает build-time defaults через compile definitions:

```cmake
target_compile_definitions(farm_association_server_core
    PUBLIC
        FARM_SERVER_BUILD_CONFIG="$<CONFIG>"
        FARM_SERVER_DEFAULT_DB_NAME="$<IF:$<CONFIG:Debug>,fasc_test,fasc_db>"
        FARM_SERVER_RESET_DATABASE_ON_START=$<IF:$<CONFIG:Debug>,1,0>
        FARM_SERVER_DOMAIN_SQL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../database/dump-mydb-sprinthost-public.sql"
        FARM_SERVER_USER_SQL_PATH="${FARM_SERVER_USER_SQL}")
```

В результате Debug executable по умолчанию готовит `fasc_test` с очисткой на каждом старте, а Release executable готовит `fasc_db` без очистки существующих данных.

## SQL Bootstrap Scripts

После сборки executable CMake копирует SQL-файлы рядом с сервером:

```text
<target-dir>/sql/farm_schema.sql
<target-dir>/sql/user.sql
```

Compile definitions указывают на source/generated SQL paths, а копия рядом с executable удобна для ручной диагностики и упаковки результата.

## Runtime DLL На Windows

Если сборка идет на Windows, CMake копирует PostgreSQL runtime DLL рядом с:

```text
farm_association_server.exe
farm_association_unit_tests.exe
farm_association_integration_tests.exe
```

Это нужно, чтобы запуск не падал с `0xc0000135`.

## Tests

`server/tests/CMakeLists.txt` создает два test executable:

```cmake
add_executable(farm_association_unit_tests)
add_executable(farm_association_integration_tests)
```

CTest получает список тестов через:

```cmake
include(GoogleTest)
gtest_discover_tests(farm_association_unit_tests TEST_PREFIX unit.)
gtest_discover_tests(farm_association_integration_tests TEST_PREFIX integration.)
```

Фактическая регистрация integration tests задает test database environment и `RESOURCE_LOCK`:

```cmake
gtest_discover_tests(farm_association_integration_tests
    TEST_PREFIX integration.
    PROPERTIES
        ENVIRONMENT "FARM_DB_NAME=fasc_test;FARM_DB_RESET_ON_START=1;FARM_DB_BOOTSTRAP=1"
        RESOURCE_LOCK fasc_test)
```

`RESOURCE_LOCK fasc_test` нужен для параллельного CTest: DB-backed tests сбрасывают одну и ту же `fasc_test`, поэтому они не должны выполнять reset одновременно.

Запуск:

```bash
ctest --test-dir server/build -C Debug --output-on-failure
```

## Как Добавить Source File

Если появился новый обычный `.cpp`, добавь его в подходящий target:

```cmake
target_sources(farm_association_server_core
    PRIVATE
        src/handling/NewEntity.cpp)
```

Если файл лежит в уже glob-покрытом каталоге `src/controllers/app`, `src/controllers/http` или `src/handling`, CMake подхватит его после повторного configure. Для важных файлов лучше предпочитать явное подключение, если порядок или особая роль должны быть очевидны.

## Частые Ошибки

`Missing local dependency path`
: bootstrap зависимостей не был выполнен или завершился неуспешно.

`Cannot find source file`
: файл переименовали или добавили, но target sources не обновлены.

`Header file not found`
: проверь include path, регистр имени файла и наличие generated files после build.

`_ITERATOR_DEBUG_LEVEL mismatch` на Windows
: Debug target пытается линковаться с Release ODB runtime или наоборот; пересобери через текущую CMake-конфигурацию, чтобы появились `libodb-Debug`/`libodb-Release`.
