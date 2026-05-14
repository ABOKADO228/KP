# Server

Этот каталог содержит C++20 backend для farm_association.

Поддерживаемая схема сборки:

```text
CMake + Ninja + clang++
```

Перед сборкой нужно подготовить `server/third_party` через bootstrap-скрипты из корня репозитория:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
```

```bash
./tools/bootstrap-deps.sh
```

Основные команды:

```powershell
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server\build
ctest --test-dir server\build -C Debug --output-on-failure
```

```bash
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build
ctest --test-dir server/build --output-on-failure
```

Краткая карта зависимостей:

```text
Boost
nlohmann/json
fmt
ODB compiler/runtime
PostgreSQL/libpq
OpenSSL
GoogleTest
```

Подробное руководство по установке зависимостей, PostgreSQL и запуску сервера находится в корневом [README.md](../README.md).
