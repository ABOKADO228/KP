# Карта Документации

Этот файл отвечает на вопрос, где в проекте искать описание конкретной части системы. Документация разделена на быстрый старт, спецификации, сборку, архитектуру, сервер, клиент и правила написания кода.

## Быстрый Маршрут Чтения

Если нужно просто запустить проект:

1. Прочитать [README.md](../README.md).
2. Подготовить зависимости по [01_DEPENDENCIES_SETUP.md](01_DEPENDENCIES_SETUP.md).
3. Собрать, запустить и проверить проект по [02_BUILD_RUN_TEST.md](02_BUILD_RUN_TEST.md).

Если нужно понять, как система устроена:

1. Прочитать [06_SYSTEM_AND_SYNTAX_GUIDE.md](06_SYSTEM_AND_SYNTAX_GUIDE.md).
2. Для backend-деталей открыть [SPECIFICATION.md](../SPECIFICATION.md) и [server/docs/module-spec.md](../server/docs/module-spec.md).
3. Для frontend-деталей открыть [client/SPECIFICATION.md](../client/SPECIFICATION.md).

Если нужно изменить код:

1. Найти слой в [06_SYSTEM_AND_SYNTAX_GUIDE.md](06_SYSTEM_AND_SYNTAX_GUIDE.md).
2. Проверить правила сборки в [03_CMAKE_GUIDE.md](03_CMAKE_GUIDE.md) для сервера или [client/README.md](../client/README.md) для клиента.
3. После изменений запустить проверки из [02_BUILD_RUN_TEST.md](02_BUILD_RUN_TEST.md).

## Документы Верхнего Уровня

| Документ | Что Описывает | Когда Читать |
| --- | --- | --- |
| [README.md](../README.md) | Общее назначение проекта, быстрый старт, зависимости, база данных, запуск, основные маршруты, клиент | Первый документ для любого разработчика |
| [SPECIFICATION.md](../SPECIFICATION.md) | Серверная спецификация: слои backend, база данных, API, ошибки, сборка, тестирование | При изменении C++ сервера или API |
| [client/README.md](../client/README.md) | Запуск React клиента, dev proxy, проверки, структура `client/src` | При работе с frontend |
| [client/SPECIFICATION.md](../client/SPECIFICATION.md) | Клиентская спецификация: роли, бизнес-модули, экраны, критерии готовности | При изменении UI, ролей и доступов |
| [server/README.md](../server/README.md) | Краткое описание backend, сборка, runtime-настройки, основные маршруты | При запуске или настройке сервера |
| [server/docs/module-spec.md](../server/docs/module-spec.md) | Серверный модуль как отдельный компонент: стек, слои, ODB, bootstrap, API model | При глубокой работе с backend |

## Документы В Каталоге `docs`

| Документ | Что Описывает |
| --- | --- |
| [00_DOCUMENTATION_MAP.md](00_DOCUMENTATION_MAP.md) | Полный список документов и маршрут чтения |
| [01_DEPENDENCIES_SETUP.md](01_DEPENDENCIES_SETUP.md) | Установка зависимостей на Windows и Manjaro, локальные third-party библиотеки |
| [02_BUILD_RUN_TEST.md](02_BUILD_RUN_TEST.md) | Команды сборки, запуска, PostgreSQL, CTest, client checks, smoke-проверки |
| [03_CMAKE_GUIDE.md](03_CMAKE_GUIDE.md) | Устройство CMake, targets, зависимости, ODB generated fallback, особенности Windows/Manjaro |
| [04_BOOST_ASIO_BEAST_SERVER.md](04_BOOST_ASIO_BEAST_SERVER.md) | HTTP-сервер на Boost.Asio/Beast: accept, session, request dispatcher |
| [05_HIGH_PERFORMANCE_BEAST_SERVER.md](05_HIGH_PERFORMANCE_BEAST_SERVER.md) | Заметки по производительному Beast server: threads, timeouts, limits |
| [06_SYSTEM_AND_SYNTAX_GUIDE.md](06_SYSTEM_AND_SYNTAX_GUIDE.md) | Как работает вся система и какой синтаксис используется во frontend/backend |

## Спецификации И Исходные Данные

| Файл | Назначение |
| --- | --- |
| [database/dump-mydb-sprinthost-public.sql](../database/dump-mydb-sprinthost-public.sql) | Исходная предметная SQL-схема и seed-данные фермерской ассоциации |
| [server/generated/persistence](../server/generated/persistence) | Checked-in ODB generated files для всех persistence-моделей; fallback для Manjaro и сборок без ODB compiler |

## Кодовые Точки Входа

| Часть | Файл | Роль |
| --- | --- | --- |
| Server entrypoint | [server/src/Main.cpp](../server/src/Main.cpp) | Читает настройки, готовит базу, создает сервисы, регистрирует маршруты, запускает сервер |
| Server core | [server/include/server/core/Server.hpp](../server/include/server/core/Server.hpp) | Публичная обертка HTTP-сервера |
| Routing | [server/include/server/core/AppRouter.hpp](../server/include/server/core/AppRouter.hpp) | Хранение и поиск HTTP-маршрутов |
| Dispatch | [server/include/server/core/RequestDispatcher.hpp](../server/include/server/core/RequestDispatcher.hpp) | Преобразование Beast request в проектный request/response |
| Database bootstrap | [server/include/database/Bootstrap.hpp](../server/include/database/Bootstrap.hpp) | Подготовка PostgreSQL базы, схем и seed-данных |
| Database facade | [server/include/database/Database.hpp](../server/include/database/Database.hpp) | Унифицированные операции select/insert/update/delete и ODB доступ |
| Auth/users | [server/include/controllers/app/Users.hpp](../server/include/controllers/app/Users.hpp) | Регистрация, логин, список пользователей, смена роли |
| Resource routes | [server/include/handling/FarmEntityRoutes.hpp](../server/include/handling/FarmEntityRoutes.hpp) | Регистрация CRUD-маршрутов предметных таблиц |
| Client entrypoint | [client/src/main.tsx](../client/src/main.tsx) | Создает React root |
| Client root | [client/src/App.tsx](../client/src/App.tsx) | Переключает auth screen и workspace |
| API client | [client/src/api/farmApi.ts](../client/src/api/farmApi.ts) | Все HTTP-запросы клиента |
| Roles/modules | [client/src/domain/roles.ts](../client/src/domain/roles.ts) | Роли, бизнес-модули, видимость и права записи |
| Resource UI | [client/src/components/ResourcePanel.tsx](../client/src/components/ResourcePanel.tsx) | Таблица, формы CRUD, поиск и фильтры |
| User admin UI | [client/src/components/UserAdministrationPanel.tsx](../client/src/components/UserAdministrationPanel.tsx) | Просмотр пользователей, создание, изменение роли |

## Документы По Типовым Задачам

| Задача | Читать |
| --- | --- |
| Запустить все с нуля | `README.md`, `docs/01_DEPENDENCIES_SETUP.md`, `docs/02_BUILD_RUN_TEST.md` |
| Исправить ошибку подключения клиента к серверу | `README.md`, `client/README.md`, `docs/02_BUILD_RUN_TEST.md` |
| Разобраться с PostgreSQL bootstrap | `README.md`, `server/README.md`, `docs/02_BUILD_RUN_TEST.md`, `docs/06_SYSTEM_AND_SYNTAX_GUIDE.md` |
| Добавить backend route | `SPECIFICATION.md`, `server/docs/module-spec.md`, `docs/06_SYSTEM_AND_SYNTAX_GUIDE.md` |
| Добавить бизнес-модуль в клиент | `client/SPECIFICATION.md`, `docs/06_SYSTEM_AND_SYNTAX_GUIDE.md` |
| Изменить роли и доступы | `README.md`, `client/SPECIFICATION.md`, `server/include/controllers/dto/User.hpp`, `client/src/domain/roles.ts` |
| Разобраться с CMake | `docs/03_CMAKE_GUIDE.md` |
| Разобраться с Beast server | `docs/04_BOOST_ASIO_BEAST_SERVER.md`, `docs/05_HIGH_PERFORMANCE_BEAST_SERVER.md` |
| Проверить Windows и Manjaro | `docs/01_DEPENDENCIES_SETUP.md`, `docs/02_BUILD_RUN_TEST.md`, `docs/03_CMAKE_GUIDE.md` |

## Актуальное Поведение Системы

- Backend - C++20 HTTP API на Boost.Asio/Beast.
- Database - PostgreSQL, bootstrap при старте, UTF-8 client encoding, предметная схема из SQL dump.
- Auth - `POST /auth/register`, `POST /auth/login`, JWT с `sub` и `role`.
- Builtin admin - `admin` / `admin12345`, роль `agriculture_admin`, создается при старте.
- User administration - `agriculture_admin` и `association_director` могут смотреть пользователей, создавать их и менять роли.
- Resource CRUD - `GET/POST /api/<resource>` и `GET/PUT/DELETE /api/<resource>/item?<key>`.
- Frontend - React + TypeScript, role-based навигация, клиентский поиск и фильтры в таблицах.
- Dev proxy - Vite проксирует `/auth`, `/users`, `/api`, `/health` в C++ server.
