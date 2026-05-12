# 01. Установка зависимостей сервера

Документ объясняет, какие зависимости нужны серверу, почему они нужны, где они должны лежать и как подготовить окружение на Windows и Manjaro.

## Главная идея

В репозитории не должны храниться большие внешние библиотеки. Поэтому каталог `server/third_party` считается локальным состоянием машины разработчика.

В Git хранится:

```text
server/third_party/.gitkeep
tools/bootstrap-deps.ps1
tools/bootstrap-deps.sh
server/cmake/FarmLocalDependencies.cmake
```

Не хранится в Git:

```text
server/third_party/boost
server/third_party/nlohmann_json
server/third_party/odb
server/third_party/postgresql
server/third_party/_sources
server/third_party/_downloads
server/third_party/_build
```

Смысл такой: исходный код проекта остается маленьким, а внешние библиотеки можно восстановить командой bootstrap.

## Какие зависимости нужны

Сервер использует:

```text
Boost.Asio       сетевой ввод-вывод
Boost.Beast      HTTP parsing/writing поверх Asio
nlohmann/json    JSON marshalling/unmarshalling
fmt              форматирование строк
ODB compiler     генерация persistence mapping
libodb           runtime ODB
libodb-pgsql     PostgreSQL backend для ODB
PostgreSQL libpq клиентская библиотека PostgreSQL
GoogleTest       unit/integration tests
```

Здесь важно различать два типа зависимостей:

```text
headers only
  Boost headers
  nlohmann/json

compiled/runtime
  fmt
  ODB compiler
  libodb
  libodb-pgsql
  libpq
  GoogleTest
```

Header-only зависимости достаточно положить в include path. Компилируемые зависимости должны либо собраться CMake-ом, либо быть доступны как готовые библиотеки.

## Windows

### Системные инструменты

Перед bootstrap нужны:

```text
Git
CMake
Ninja
clang++
PowerShell
```

Проверка:

```powershell
git --version
cmake --version
ninja --version
clang++ --version
```

Синтаксис:

```powershell
program --version
```

Это запускает программу `program` и передает ей аргумент `--version`. Если команда не найдена, значит программа не установлена или не добавлена в `PATH`.

### Bootstrap

Из корня репозитория:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
```

Что означает команда:

```text
powershell
  запускает PowerShell

-ExecutionPolicy Bypass
  разрешает выполнить локальный скрипт в рамках этой команды

-File tools\bootstrap-deps.ps1
  указывает файл скрипта, который нужно выполнить
```

Скрипт скачивает архивы в:

```text
server/third_party/_downloads
```

Распаковывает исходники в:

```text
server/third_party/_sources
```

И подготавливает рабочие каталоги:

```text
server/third_party/boost
server/third_party/nlohmann_json
server/third_party/odb
server/third_party/postgresql
```

GoogleTest остается в:

```text
server/third_party/_sources/googletest-1.17.0
```

CMake подключает его через `add_subdirectory`, поэтому отдельно устанавливать GoogleTest в систему не нужно.

## Manjaro

### Системные пакеты

```bash
sudo pacman -S --needed base-devel cmake ninja clang curl unzip postgresql postgresql-libs
```

Синтаксис:

```text
sudo
  выполнить команду с правами администратора

pacman
  пакетный менеджер Arch/Manjaro

-S
  установить пакеты

--needed
  не переустанавливать уже установленные пакеты
```

Пакеты:

```text
base-devel       компиляторные и build-инструменты
cmake            генератор build-системы
ninja            быстрая build-система
clang            C/C++ компилятор
curl             скачивание файлов
unzip            распаковка zip
postgresql       сервер PostgreSQL и утилиты
postgresql-libs  libpq и заголовки клиента PostgreSQL
```

### Bootstrap

```bash
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh
```

Синтаксис:

```text
chmod +x file
  добавить файлу право на запуск

./tools/bootstrap-deps.sh
  выполнить shell-скрипт из текущего репозитория
```

Linux-скрипт скачивает Boost, nlohmann/json, fmt, ODB compiler, libodb, libodb-pgsql и GoogleTest. Для PostgreSQL он использует системный `libpq`, потому что на Manjaro это нормальная пакетная зависимость.

## Проверка результата

После bootstrap должны существовать:

```bash
ls server/third_party
ls server/third_party/_sources
```

На Windows:

```powershell
Get-ChildItem server\third_party
Get-ChildItem server\third_party\_sources
```

Если CMake сообщает, что не найден `GoogleTest sources`, `odb`, `libpq-fe.h` или `boost/version.hpp`, значит bootstrap не был выполнен или завершился неуспешно.

## Git-контроль

Перед push:

```bash
git ls-files server/third_party
```

Ожидаемый результат:

```text
server/third_party/.gitkeep
```

Если появились другие файлы:

```bash
git rm -r --cached server/third_party
git add server/third_party/.gitkeep
```

`--cached` означает: убрать файл из Git-индекса, но оставить его на диске.
## Как читать bootstrap-скрипты

Bootstrap-скрипт - это не магия, а обычная автоматизация повторяющихся действий:

```text
1. создать каталоги
2. скачать архивы
3. распаковать архивы
4. переложить нужные include/bin/lib файлы в ожидаемые места
5. оставить исходники там, где CMake сможет их подключить
```

В PowerShell-скрипте важны функции:

```powershell
function Ensure-Directory($Path) { ... }
function Download-File($Url, $Output) { ... }
function Expand-Zip($Archive, $Destination) { ... }
function Copy-Directory($From, $To) { ... }
```

Синтаксис PowerShell function:

```powershell
function Name($arg1, $arg2) {
    # commands
}
```

Переменная в PowerShell начинается с `$`:

```powershell
$Deps = Join-Path $Root "server\third_party"
```

`Join-Path` нужен, чтобы собрать путь корректно для Windows. Это лучше, чем вручную склеивать строки через `\`.

В bash-скрипте аналогичные задачи решаются функциями:

```bash
need_cmd() { ... }
download() { ... }
extract_zip() { ... }
extract_tar_bz2() { ... }
```

Переменная читается через `$NAME`:

```bash
DEPS="$ROOT/server/third_party"
```

Кавычки важны. Без кавычек путь с пробелами может развалиться на несколько аргументов.

## Почему зависимости лежат именно так

CMake ожидает конкретную структуру:

```text
server/third_party/boost/include/boost/version.hpp
server/third_party/nlohmann_json/include/nlohmann/json.hpp
server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
server/third_party/odb/bin/odb
server/third_party/postgresql/include/libpq-fe.h
```

На Windows ODB compiler обычно лежит как:

```text
server/third_party/odb/bin/odb.exe
```

На Manjaro:

```text
server/third_party/odb/bin/odb
```

PostgreSQL runtime на Windows включает DLL из:

```text
server/third_party/postgresql/bin
```

Если `.exe` запускается без нужной DLL, Windows может завершить процесс с ошибкой `0xc0000135`.

## Как CMake проверяет зависимости

В `server/cmake/FarmLocalDependencies.cmake` есть функция:

```cmake
function(farm_require_path path description)
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR
            "Missing local dependency path: ${path}\n"
            "Expected: ${description}\n"
            "Install the dependency inside server/third_party; system packages are intentionally not used.")
    endif()
endfunction()
```

Разбор:

```text
function(...)
  объявление функции CMake

if(NOT EXISTS "${path}")
  проверить существование файла или каталога

message(FATAL_ERROR ...)
  остановить конфигурацию CMake с ошибкой

${path}
  подстановка значения переменной
```

Такой подход полезен: ошибка появляется рано, на этапе configure, и сразу говорит, какой путь отсутствует.

## Типовые ошибки Windows

### PowerShell запрещает запуск скрипта

Симптом:

```text
running scripts is disabled on this system
```

Решение для одного запуска:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
```

### clang++ не найден

Проверка:

```powershell
where.exe clang++
clang++ --version
```

`where.exe` ищет executable в `PATH`.

### Ninja не найден

Проверка:

```powershell
where.exe ninja
ninja --version
```

Если Ninja не найден, CMake с `-G Ninja` не сможет создать build files.

### Ошибка `0xc0000135`

Чаще всего рядом с executable нет нужной DLL. Проверь:

```powershell
Get-ChildItem server\build -Filter *.dll
Get-ChildItem server\build\tests -Filter *.dll
```

## Типовые ошибки Manjaro

### `libpq-fe.h` не найден

Решение:

```bash
sudo pacman -S --needed postgresql-libs
./tools/bootstrap-deps.sh
```

### `permission denied` при запуске bootstrap

Решение:

```bash
chmod +x tools/bootstrap-deps.sh
```

`+x` добавляет execute bit.

### Нужно принудительно скачать зависимости заново

Windows:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1 -Force
```

Manjaro:

```bash
FORCE=1 ./tools/bootstrap-deps.sh
```

`FORCE=1` задает переменную окружения только для одного запуска команды.

## Контрольный список после установки

Проверь:

```text
1. cmake --version работает
2. ninja --version работает
3. clang++ --version работает
4. boost/version.hpp существует
5. nlohmann/json.hpp существует
6. fmt CMakeLists.txt существует
7. googletest CMakeLists.txt существует
8. odb или odb.exe существует
9. libpq-fe.h существует
10. git ls-files server/third_party показывает только .gitkeep
```

Windows:

```powershell
Test-Path server\third_party\boost\include\boost\version.hpp
Test-Path server\third_party\nlohmann_json\include\nlohmann\json.hpp
Test-Path server\third_party\_sources\fmt-12.1.0\CMakeLists.txt
Test-Path server\third_party\_sources\googletest-1.17.0\CMakeLists.txt
Test-Path server\third_party\postgresql\include\libpq-fe.h
git ls-files server/third_party
```

Manjaro:

```bash
test -f server/third_party/boost/include/boost/version.hpp
test -f server/third_party/nlohmann_json/include/nlohmann/json.hpp
test -f server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
test -f server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
test -f server/third_party/postgresql/include/libpq-fe.h
git ls-files server/third_party
```
