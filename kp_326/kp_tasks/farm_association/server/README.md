# Farm Association Server

C++20 сервер на Boost.Beast, nlohmann/json и ODB PostgreSQL. Проект собирается через CMake без vcpkg: все внешние зависимости находятся локально в `server/third_party`.

## Требования

Нужны установленные инструменты:

```powershell
cmake --version
ninja --version
clang++ --version
```

На Windows также можно собирать генератором Visual Studio, но основной проверенный вариант для проекта:

```text
CMake + Ninja + clang++
```

## Локальные зависимости

Перед сборкой каталог `server/third_party` должен содержать:

```text
server/
  third_party/
    boost/
      include/boost/version.hpp
      lib/
    nlohmann_json/
      include/nlohmann/json.hpp
    odb/
      bin/odb.exe
    postgresql/
      include/libpq-fe.h
      lib/
      bin/
    _sources/
      libodb-2.4.0/
      libodb-pgsql-2.4.0/
```

`libodb` и `libodb-pgsql` вручную собирать каждый раз не нужно. Основная сборка сама один раз соберет их из `server/third_party/_sources` и положит готовые архивы в:

```text
server/third_party/libodb/lib
server/third_party/libodb-pgsql/lib
```

Повторные запуски `cmake --build` будут использовать уже готовые файлы. Если нужно полностью пересобрать эти внешние библиотеки, удали каталог `server/third_party/_build/odb-local` и файлы из `server/third_party/libodb/lib`, `server/third_party/libodb-pgsql/lib`.

## Конфигурация

Из корня проекта:

```powershell
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

На Linux/macOS команда такая же, только путь к build-каталогу обычно пишут через `/`:

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

Если нужно временно собрать сервер без ODB:

```powershell
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug -DFARM_SERVER_WITH_ODB=OFF
```

Если `libodb` и `libodb-pgsql` уже собраны заранее и не нужно запускать локальную сборку зависимостей:

```powershell
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug -DFARM_SERVER_BUILD_LOCAL_ODB_DEPS=OFF
```

## Сборка

```powershell
cmake --build server\build
```

При первом запуске сборка дополнительно соберет локальные ODB runtime-библиотеки. Дальше они будут переиспользоваться.

Для Linux/macOS:

```bash
cmake --build server/build
```

## Запуск

Проверить версию:

```powershell
.\server\build\farm_association_server.exe --version
```

Запустить сервер:

```powershell
.\server\build\farm_association_server.exe
```

На Linux/macOS:

```bash
./server/build/farm_association_server --version
./server/build/farm_association_server
```

## Тесты

```powershell
ctest --test-dir server\build --output-on-failure
```

На Linux/macOS:

```bash
ctest --test-dir server/build --output-on-failure
```

Сейчас тест проверяет smoke-запуск `farm_association_server --version`.
