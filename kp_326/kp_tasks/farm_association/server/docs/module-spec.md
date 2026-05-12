# Спецификация модуля: C++ сервер ассоциации фермерских хозяйств

## Назначение

Модуль `server` содержит C++20 HTTP-сервер для backend-части проекта. Сервер должен принимать HTTP-запросы, сериализовать ответы в JSON, работать с PostgreSQL через ORM ODB и собираться через CMake, Ninja и clang++.

## Технологический стек

- Язык: C++20.
- Сборка: CMake + Ninja.
- Компилятор: clang++.
- Тесты: CTest.
- HTTP: Boost.Beast.
- JSON: nlohmann/json.
- ORM: ODB.
- База данных: PostgreSQL, клиентская библиотека `libpq`.

## Политика зависимостей

Зависимости устанавливаются локально внутрь проекта, без vcpkg:

```text
server/
  third_party/
    boost/
    nlohmann_json/
    odb/
    libodb/
    libodb-pgsql/
    postgresql/
```

`third_party` является локальным каталогом зависимостей сервера. CMake ищет пакеты только через локальные префиксы из `server/third_party` и стандартные системные пути CMake. Предпочтительный режим работы: все нужные заголовки, библиотеки и инструменты лежат внутри `server/third_party`.

## Роли зависимостей

- Boost установлен локально полностью в `server/third_party/boost`, включая общий каталог заголовков `include/boost` и собранные статические `.lib` файлы в `lib`.
- Boost.Beast предоставляет HTTP/WebSocket-примитивы поверх Boost.Asio.
- Boost.System может использоваться как собранная библиотека или как header-only error_code через текущую настройку CMake.
- nlohmann/json используется для JSON-тел запросов и ответов.
- ODB compiler (`odb.exe`) генерирует C++ код доступа к данным из persistent-классов.
- `libodb` содержит базовый runtime ODB.
- `libodb-pgsql` содержит PostgreSQL-драйвер ODB.
- PostgreSQL `libpq` используется драйвером ODB PostgreSQL.

## Сборочная модель

CMake-файл сервера:

- задает стандарт C++20;
- создает исполняемый файл `farm_association_server`;
- подключает локальные зависимости через `CMAKE_PREFIX_PATH`;
- подключает локальные заголовки Boost и nlohmann/json из `server/third_party`;
- добавляет `server/third_party/boost/lib` в link directories для будущих компонентов Boost;
- при включенной опции `FARM_SERVER_WITH_ODB=ON` ищет `odb.exe`, `libodb`, `libodb-pgsql` и `libpq`;
- регистрирует smoke-тест `farm_association_server_version` для CTest.

## ODB workflow

Для классов модели, которые должны храниться в PostgreSQL, используется схема:

1. Описать persistent-класс в `include/...`.
2. Запустить `odb.exe` с параметрами `--database pgsql`, `--std c++20`, `--generate-query`, `--generate-schema`.
3. Добавить сгенерированные `.cxx` файлы в сборку.
4. Применить сгенерированную SQL-схему к PostgreSQL.

Генерацию ODB-кода лучше добавить в CMake после появления первой модели данных, чтобы не держать в проекте абстрактную команду без реального входного файла.

## Текущий статус

- Подготовлен каркас CMake для локальных зависимостей без vcpkg.
- Boost 1.90.0 скачан полностью, распакован и установлен локально в `server/third_party/boost`.
- Добавлен минимальный `src/main.cpp`, который проверяет подключение Boost.Beast и nlohmann/json.
- ODB включен в сборку через `FARM_SERVER_WITH_ODB=ON`, но фактическая сборка требует локально установленного `odb.exe`, `libodb`, `libodb-pgsql` и `libpq`.
