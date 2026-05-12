# Farm Association

C++20 серверная часть проекта на Boost.Asio, Boost.Beast, nlohmann/json, ODB и PostgreSQL.

Проект собирается через CMake. Внешние зависимости не хранятся в Git: они скачиваются в локальный каталог `server/third_party` bootstrap-скриптами. В репозитории должны оставаться только исходники, CMake-файлы, тесты, скрипты и документация.

## Быстрый старт

Windows:

```powershell
powershell -ExecutionPolicy Bypass -File tools\bootstrap-deps.ps1
cmake -S server -B server\build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server\build
ctest --test-dir server\build -C Debug --output-on-failure
```

Manjaro:

```bash
sudo pacman -S --needed base-devel cmake ninja clang curl unzip postgresql postgresql-libs
chmod +x tools/bootstrap-deps.sh
./tools/bootstrap-deps.sh
cmake -S server -B server/build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build server/build
ctest --test-dir server/build --output-on-failure
```

## Запуск

Сервер слушает порт `8080`.

Windows:

```powershell
.\server\build\farm_association_server.exe
```

Manjaro:

```bash
./server/build/farm_association_server
```

Проверка:

```bash
curl http://localhost:8080/health
curl -X POST http://localhost:8080/users -H "Content-Type: application/json" -d '{"name":"Alex"}'
```

## База данных

Сервер читает настройки PostgreSQL из переменных окружения:

```text
FARM_DB_USER      postgres
FARM_DB_PASSWORD  password
FARM_DB_NAME      farm_association
FARM_DB_HOST      localhost
FARM_DB_PORT      5432
```

SQL-схема генерируется ODB во время сборки:

```text
server/build/generated/persistence/user.sql
```

Перед успешным `POST /users` база `farm_association` должна существовать, а схема должна быть применена через `psql`.

## Тесты

Тесты написаны на GoogleTest и запускаются через CTest:

```bash
ctest --test-dir server/build --output-on-failure
```

Структура тестов:

```text
server/tests/unit/controller
server/tests/unit/marshalling
server/tests/integration/endpoints
server/tests/integration/business
server/tests/fixtures
server/tests/utils
```

## Документация

- `docs/01_DEPENDENCIES_SETUP.md` - подробная установка зависимостей на Windows и Manjaro.
- `docs/02_BUILD_RUN_TEST.md` - сборка, запуск, PostgreSQL и тесты.
- `docs/03_CMAKE_GUIDE.md` - как устроен CMake в проекте и что означает каждая важная команда.
- `docs/04_BOOST_ASIO_BEAST_SERVER.md` - как работает HTTP-сервер на Boost.Asio и Boost.Beast.

Архив старых корневых документов сохранен здесь:

```text
docs/archive/root-md-before-methodical-docs-2026-05-13.zip
```

## Git policy

Проверка перед push:

```bash
git ls-files server/third_party
```

Ожидаемый вывод:

```text
server/third_party/.gitkeep
```

Если в Git попали внешние зависимости, их нужно убрать из индекса, не удаляя локальные файлы:

```bash
git rm -r --cached server/third_party
git add server/third_party/.gitkeep
```
