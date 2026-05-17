# 01. Установка Зависимостей Сервера

Документ описывает зависимости C++ сервера и текущий способ подготовки окружения на Windows и Manjaro.

## Общая Идея

Большие внешние библиотеки не хранятся в Git. Репозиторий содержит только скрипты подготовки и `server/third_party/.gitkeep`, а локальная машина разработчика восстанавливает зависимости командой bootstrap.

В Git должны быть:

```text
server/third_party/.gitkeep
tools/bootstrap-deps.ps1
tools/bootstrap-deps.sh
server/cmake/FarmLocalDependencies.cmake
```

В Git не должны попадать:

```text
server/third_party/boost
server/third_party/nlohmann_json
server/third_party/odb
server/third_party/postgresql
server/third_party/openssl
server/third_party/libodb-*
server/third_party/libodb-pgsql-*
server/third_party/_sources
server/third_party/_downloads
server/third_party/_build
```

## Что Нужно Проекту

```text
Boost.Asio/Beast  HTTP server
nlohmann/json     JSON marshalling
fmt               formatting
ODB compiler      generation of persistence mapping
libodb            ODB runtime
libodb-pgsql      ODB PostgreSQL driver
PostgreSQL libpq  PostgreSQL client library
OpenSSL/libcrypto JWT/password/security dependencies
GoogleTest        unit/integration tests
```

Boost и nlohmann/json используются как headers. fmt и GoogleTest подключаются из исходников через CMake. ODB compiler скачивается bootstrap-ом, а `libodb`/`libodb-pgsql` собираются CMake-ом при первом build.

## Windows

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

Bootstrap из корня репозитория:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
```

Принудительное обновление:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1 -Force
```

После bootstrap ожидаются:

```text
server/third_party/boost/include/boost/version.hpp
server/third_party/nlohmann_json/include/nlohmann/json.hpp
server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
server/third_party/odb/bin/odb.exe
server/third_party/postgresql/include/libpq-fe.h
server/third_party/postgresql/include/openssl/evp.h
server/third_party/postgresql/lib/libpq.lib
server/third_party/postgresql/lib/libcrypto.lib
```

`libodb` и `libodb-pgsql` появятся после `cmake --build`:

```text
server/third_party/libodb-Debug
server/third_party/libodb-pgsql-Debug
server/third_party/libodb-Release
server/third_party/libodb-pgsql-Release
```

Разделение по конфигурациям важно на Windows: Debug и Release используют разные runtime-настройки компилятора, поэтому один общий `.lib` может привести к ошибкам линковки.

## Manjaro

Установи системные пакеты:

```bash
sudo pacman -S --needed base-devel cmake ninja clang curl unzip tar openssl postgresql postgresql-libs
```

Bootstrap:

```bash
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh
```

Принудительное обновление:

```bash
FORCE=1 ./tools/bootstrap-deps.sh
```

Linux-скрипт скачивает Boost, nlohmann/json, fmt, ODB compiler и GoogleTest. PostgreSQL/libpq и OpenSSL берутся из системных пакетов; CMake использует их напрямую, а bootstrap может подготовить локальную копию в `server/third_party` без подмешивания Windows-бандла PostgreSQL в Linux include path.

После bootstrap ожидаются:

```text
server/third_party/boost/include/boost/version.hpp
server/third_party/nlohmann_json/include/nlohmann/json.hpp
server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
server/third_party/odb/bin/odb
server/third_party/postgresql/include/libpq-fe.h
server/third_party/postgresql/lib/libpq.so
server/third_party/openssl/include/openssl/evp.h
server/third_party/openssl/lib/libcrypto.so
```

`libodb` и `libodb-pgsql` также собираются CMake-ом при первом build. На Linux итоговые каталоги имеют конфигурационное имя только если это нужно текущему генератору и toolchain; проект не требует ручной установки ODB runtime в систему.

## Проверка

Windows:

```powershell
Test-Path server\third_party\boost\include\boost\version.hpp
Test-Path server\third_party\nlohmann_json\include\nlohmann\json.hpp
Test-Path server\third_party\_sources\fmt-12.1.0\CMakeLists.txt
Test-Path server\third_party\_sources\googletest-1.17.0\CMakeLists.txt
Test-Path server\third_party\odb\bin\odb.exe
Test-Path server\third_party\postgresql\include\libpq-fe.h
Test-Path server\third_party\postgresql\include\openssl\evp.h
Test-Path server\third_party\postgresql\lib\libpq.lib
Test-Path server\third_party\postgresql\lib\libcrypto.lib
```

Manjaro:

```bash
test -f server/third_party/boost/include/boost/version.hpp
test -f server/third_party/nlohmann_json/include/nlohmann/json.hpp
test -f server/third_party/_sources/fmt-12.1.0/CMakeLists.txt
test -f server/third_party/_sources/googletest-1.17.0/CMakeLists.txt
test -x server/third_party/odb/bin/odb
test -f server/third_party/postgresql/include/libpq-fe.h
test -f server/third_party/postgresql/lib/libpq.so
test -f server/third_party/openssl/include/openssl/evp.h
test -f server/third_party/openssl/lib/libcrypto.so
```

Git-проверка:

```bash
git ls-files server/third_party
```

Ожидаемый вывод:

```text
server/third_party/.gitkeep
```

Если появились другие tracked-файлы из `third_party`, убери их из индекса:

```bash
git rm -r --cached server/third_party
git add server/third_party/.gitkeep
```

## Частые Ошибки

`Missing local dependency path`
: bootstrap не был выполнен или завершился неуспешно.

`odb` не найден
: проверь `server/third_party/odb/bin/odb` или `server/third_party/odb/bin/odb.exe`.

`openssl/evp.h` или `libcrypto` не найдены на Manjaro
: установи `openssl` и повтори `FORCE=1 ./tools/bootstrap-deps.sh`.

`libpq-fe.h` или `libpq` не найдены на Manjaro
: установи `postgresql-libs` и повтори bootstrap.

Windows error `0xc0000135`
: рядом с executable нет нужной DLL. Пересобери проект: CMake копирует PostgreSQL runtime DLL рядом с сервером и тестами после build.
