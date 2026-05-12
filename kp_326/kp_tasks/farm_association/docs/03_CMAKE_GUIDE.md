# 03. Методичка по CMake проекта

Документ объясняет, как устроен `server/CMakeLists.txt`, зачем нужны targets, options, include directories, generated files и tests.

## Что такое CMake в этом проекте

CMake не компилирует C++ сам. Он генерирует build-систему.

В нашем случае:

```text
CMakeLists.txt -> Ninja files -> clang++ -> .obj/.lib/.exe
```

Команда:

```bash
cmake -S server -B server/build -G Ninja
```

создает в `server/build` файлы, которые понимает Ninja.

Команда:

```bash
cmake --build server/build
```

запускает уже сгенерированную build-систему.

## Минимальная структура CMakeLists

В начале:

```cmake
cmake_minimum_required(VERSION 3.25)
project(farm_association_server VERSION 0.1.0 LANGUAGES CXX)
```

Смысл:

```text
cmake_minimum_required
  фиксирует минимальную версию CMake и набор доступных возможностей

project
  задает имя проекта, версию и языки
```

Дальше:

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

Это означает:

```text
CMAKE_CXX_STANDARD 20
  нужен C++20

CMAKE_CXX_STANDARD_REQUIRED ON
  нельзя молча понизить стандарт

CMAKE_CXX_EXTENSIONS OFF
  использовать стандартный C++ без compiler-specific extensions
```

## Options

В проекте есть options:

```cmake
option(FARM_SERVER_WITH_ODB "Build with ODB PostgreSQL support" ON)
option(FARM_SERVER_ODB_STATIC "Link ODB runtime as static libraries" ON)
option(FARM_SERVER_BUILD_LOCAL_ODB_DEPS "..." ON)
option(FARM_SERVER_BUILD_TESTS "Build unit and integration tests" ON)
```

`option` создает boolean CMake cache variable.

Использование:

```bash
cmake -S server -B server/build -DFARM_SERVER_BUILD_TESTS=OFF
```

Синтаксис:

```text
-DNAME=value
  записать value в CMake cache variable NAME
```

## Targets

Главная идея modern CMake: собирать не "файлы вообще", а targets.

В проекте есть:

```text
farm_association_server_core
farm_association_server
farm_association_unit_tests
farm_association_integration_tests
```

`farm_association_server_core`:

```cmake
add_library(farm_association_server_core STATIC)
```

Это статическая библиотека с основной серверной логикой. В нее входят controllers, handlers, database layer, router, dispatcher и server core.

`farm_association_server`:

```cmake
add_executable(farm_association_server)
```

Это executable с `main.cpp`.

Почему нужен `core` target:

```text
main.cpp не должен попадать в unit tests
tests должны линковаться к логике сервера
общая логика не должна дублироваться
```

## target_sources

Пример:

```cmake
target_sources(farm_association_server_core
    PRIVATE
        src/controllers/app/Users.cpp
        src/controllers/http/Users.cpp
        src/database/database.cpp
        src/handling/Health.cpp
        src/handling/Users.cpp
        src/server/core/AppRouter.cpp
        src/server/core/RequestDispatcher.cpp
        src/server/core/Server.cpp
        "${FARM_SERVER_USER_ODB_CXX}")
```

Смысл:

```text
target_sources
  добавить source files к target

PRIVATE
  эти файлы нужны только для сборки самого target
```

## target_include_directories

Пример:

```cmake
target_include_directories(farm_association_server_core
    PUBLIC
        "${FARM_SERVER_GENERATED_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/persistence"
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
```

Смысл:

```text
PUBLIC
  include directories нужны и самому target, и тем, кто к нему линкуется
```

Это важно для тестов: они линкуются к `farm_association_server_core`, значит автоматически получают include paths сервера.

## target_link_libraries

Пример:

```cmake
target_link_libraries(farm_association_server_core
    PUBLIC
        Threads::Threads
        farm_boost
        farm_nlohmann_json
        farm_fmt)
```

Смысл:

```text
target_link_libraries
  связать target с библиотеками

PUBLIC
  зависимость нужна и этому target, и пользователям этого target
```

Если тесты используют headers, которые требуют Boost или nlohmann/json, эти зависимости приходят через `farm_association_server_core`.

## Generated files ODB

ODB читает:

```text
server/include/persistence/User.hpp
```

И генерирует:

```text
server/build/generated/persistence/user-odb.cxx
server/build/generated/persistence/user-odb.hxx
server/build/generated/persistence/user-odb.ixx
server/build/generated/persistence/user.sql
```

CMake описывает это через:

```cmake
add_custom_command(
    OUTPUT
        "${FARM_SERVER_USER_ODB_CXX}"
        "${FARM_SERVER_USER_ODB_HXX}"
        "${FARM_SERVER_USER_ODB_IXX}"
        "${FARM_SERVER_USER_SQL}"
    COMMAND "${FARM_SERVER_ODB_COMPILER}"
        --std c++14
        --database pgsql
        --generate-query
        --generate-schema
        --output-dir "${FARM_SERVER_GENERATED_PERSISTENCE_DIR}"
        --input-name user
        "${FARM_SERVER_USER_MODEL}"
)
```

Смысл ключевых параметров ODB:

```text
--database pgsql
  генерировать код под PostgreSQL

--generate-query
  сгенерировать query API

--generate-schema
  сгенерировать SQL-схему

--output-dir
  куда положить generated files

--input-name user
  использовать stable lowercase имя для generated files,
  хотя исходный файл называется User.hpp
```

`--input-name user` нужен, чтобы стиль исходников был `User.hpp`, а generated files оставались `user-odb.*`.

## Local dependencies helper

Файл:

```text
server/cmake/FarmLocalDependencies.cmake
```

содержит функции:

```text
farm_add_boost_headers
farm_add_nlohmann_json
farm_add_fmt
farm_add_googletest
farm_add_odb_local_build
farm_find_local_program
farm_find_local_library
```

CMake function выглядит так:

```cmake
function(name arg1 arg2)
    ...
endfunction()
```

Вызов:

```cmake
farm_add_googletest("${FARM_SERVER_DEPS_DIR}")
```

Эта функция проверяет, что GoogleTest скачан, отключает лишнее и подключает его через `add_subdirectory`.

## Tests

В `server/tests/CMakeLists.txt`:

```cmake
add_executable(farm_association_unit_tests)
add_executable(farm_association_integration_tests)
```

Тесты подключаются так:

```cmake
target_link_libraries(farm_association_unit_tests
    PRIVATE
        farm_association_server_core
        GTest::gtest_main)
```

`GTest::gtest_main` дает готовый `main()` для тестового executable.

CTest получает список тестов через:

```cmake
include(GoogleTest)
gtest_discover_tests(farm_association_unit_tests TEST_PREFIX unit.)
gtest_discover_tests(farm_association_integration_tests TEST_PREFIX integration.)
```

`gtest_discover_tests` запускает test executable на этапе build/discovery и спрашивает GoogleTest список тестов.

На Windows тестам нужны PostgreSQL DLL, потому что они линкуются к server core:

```cmake
add_custom_command(TARGET ${test_target} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${FARM_SERVER_DEPS_DIR}/postgresql/bin"
        "$<TARGET_FILE_DIR:${test_target}>")
```

`POST_BUILD` означает: выполнить команду после сборки target.
## PUBLIC, PRIVATE и INTERFACE подробнее

В modern CMake зависимости распространяются через targets. Поэтому почти у каждой команды есть область видимости:

```text
PRIVATE
  нужно только самому target

PUBLIC
  нужно самому target и всем, кто к нему линкуется

INTERFACE
  не нужно самому target, но нужно пользователям target
```

Пример:

```cmake
target_include_directories(farm_association_server_core
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
```

`PUBLIC` здесь нужен, потому что:

```text
farm_association_server_core собирает свои .cpp и должен видеть headers
farm_association_unit_tests линкуется к core и тоже должен видеть headers
farm_association_integration_tests линкуется к core и тоже должен видеть headers
```

Если поставить `PRIVATE`, core соберется, но тесты могут перестать видеть headers.

Пример `INTERFACE`:

```cmake
add_library(farm_nlohmann_json INTERFACE)
target_include_directories(farm_nlohmann_json INTERFACE "${include_dir}")
```

У `farm_nlohmann_json` нет своих `.cpp`; это target-обертка над include directory.

## Cache variables

CMake cache - это файл:

```text
server/build/CMakeCache.txt
```

Когда ты пишешь:

```bash
cmake -S server -B server/build -DFARM_SERVER_BUILD_TESTS=OFF
```

CMake сохраняет:

```text
FARM_SERVER_BUILD_TESTS:BOOL=OFF
```

При следующем запуске configure значение останется `OFF`, даже если ты не передашь `-D` снова.

Чтобы изменить значение:

```bash
cmake -S server -B server/build -DFARM_SERVER_BUILD_TESTS=ON
```

Чтобы начать совсем чисто, удалить build directory:

Windows:

```powershell
Remove-Item server\build -Recurse -Force
```

Manjaro:

```bash
rm -rf server/build
```

## Generator expressions

В CMake есть выражения вида:

```cmake
$<TARGET_FILE_DIR:farm_association_server>
```

Это generator expression. Оно вычисляется не сразу при чтении `CMakeLists.txt`, а на этапе генерации/build.

Смысл:

```text
$<TARGET_FILE_DIR:target>
  каталог, где лежит собранный executable/library target
```

В проекте это используется для копирования PostgreSQL DLL:

```cmake
add_custom_command(TARGET farm_association_server POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${FARM_SERVER_DEPS_DIR}/postgresql/bin"
        "$<TARGET_FILE_DIR:farm_association_server>")
```

## Imported libraries

ODB и libpq подключаются как imported libraries:

```cmake
farm_add_imported_library(ODB::odb "${ODB_LIBRARY}" "${ODB_INCLUDE_DIR}")
```

Imported library - это target, который описывает уже существующую библиотеку на диске.

У него есть:

```text
IMPORTED_LOCATION
  путь к .lib/.a/.so

INTERFACE_INCLUDE_DIRECTORIES
  headers, которые нужны пользователям библиотеки
```

Это лучше, чем вручную добавлять `-I` и `.lib` в разные места.

## add_custom_command и dependency graph

ODB generation описан через `add_custom_command`.

Ключевые части:

```cmake
OUTPUT generated-file
COMMAND tool args...
DEPENDS input-file tool
```

Смысл:

```text
OUTPUT
  какие файлы появятся после выполнения команды

COMMAND
  какую команду выполнить

DEPENDS
  от каких файлов зависит результат
```

Если `User.hpp` изменился, Ninja видит, что generated `user-odb.cxx` устарел, и запускает ODB снова.

## Почему generated directory добавлен в include path

ODB генерирует header:

```text
server/build/generated/persistence/user-odb.hxx
```

Код включает его так:

```cpp
#include <persistence/user-odb.hxx>
```

Чтобы такой include работал, CMake добавляет:

```cmake
"${FARM_SERVER_GENERATED_DIR}"
```

Если `FARM_SERVER_GENERATED_DIR` равен:

```text
server/build/generated
```

то include:

```text
persistence/user-odb.hxx
```

находит файл:

```text
server/build/generated/persistence/user-odb.hxx
```

## Почему source headers в PascalCase, а generated ODB в lowercase

Исходная модель:

```text
server/include/persistence/User.hpp
```

Сгенерированные файлы:

```text
user-odb.cxx
user-odb.hxx
user-odb.ixx
user.sql
```

Это сделано через:

```cmake
--input-name user
```

Причина: стиль исходников проекта использует `User.hpp`, но generated files удобнее оставить стабильными lowercase.

## Как добавить новый исходный файл

Допустим, появился:

```text
server/src/handling/Products.cpp
server/include/handling/Products.hpp
```

Нужно добавить `.cpp` в target:

```cmake
target_sources(farm_association_server_core
    PRIVATE
        src/handling/Products.cpp)
```

Header `.hpp` обычно не нужно добавлять в `target_sources`, если он не нужен IDE. Компилятор найдет его через include directory.

## Как добавить новый test file

Unit test controller-слоя:

```text
server/tests/unit/controller/ProductControllerTests.cpp
```

Добавить в:

```cmake
target_sources(farm_association_unit_tests
    PRIVATE
        unit/controller/ProductControllerTests.cpp)
```

Integration endpoint test:

```text
server/tests/integration/endpoints/ProductEndpointTests.cpp
```

Добавить в:

```cmake
target_sources(farm_association_integration_tests
    PRIVATE
        integration/endpoints/ProductEndpointTests.cpp)
```

## Частые CMake ошибки

### Missing local dependency path

Причина: bootstrap не подготовил зависимость.

Решение:

```bash
./tools/bootstrap-deps.sh
```

Windows:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
```

### Cannot find source file

Причина: файл переименовали, но не обновили `target_sources`.

Проверить:

```bash
rg "OldFileName" server/CMakeLists.txt server
```

### Header file not found

Причины:

```text
1. include path не добавлен
2. include написан с неправильным регистром
3. файл лежит в другом каталоге
4. generated file еще не создан
```

На Manjaro регистр важен:

```text
Users.hpp != users.hpp
User.hpp != user.hpp
```
