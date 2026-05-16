# Спецификация сервера фермерской ассоциации

## Цель

Нужно развить существующий C++20 сервер приложения фермерской ассоциации. Сервер предоставляет HTTP API, работает с PostgreSQL базой данных и реализует операции над таблицами из дампа `database/dump-mydb-sprinthost-public.sql`.

Серверная часть остается в каталоге `server`. Все внешние зависимости используются из локального каталога `server/third_party`; новые внешние менеджеры пакетов не подключаются.

## Текущая основа проекта

В проекте уже есть:

- CMake-сборка `server/CMakeLists.txt`.
- HTTP-слой на Boost.Asio/Boost.Beast.
- JSON через `nlohmann/json`.
- PostgreSQL/ODB слой через `fasc::server::database::Database`.
- Обертка транзакций `fasc::server::database::Transaction`.
- Пользовательские маршруты `POST /auth/register`, `POST /auth/login`, `POST /users`, `GET /users`, `PUT /users/role`.
- CRUD-контроллеры, handlers, views и marshalling для таблиц предметной области из SQL-дампа.
- JWT с claim `sub` и `role`, хеширование паролей в `fasc::server::security`.

Текущий маршрут запроса:

```text
Boost.Beast request
  -> RequestDispatcher
  -> AppRouter
  -> Handler
  -> HTTP controller
  -> Application controller
  -> Database / ODB / PostgreSQL
```

Эту схему нужно сохранить.

## Требования к стилю

Код пишется в существующем стиле проекта.

- Публичные классы, структуры, методы и важные поля документируются Doxygen-комментариями.
- Формат комментариев сохраняется как в текущих файлах:

```cpp
///Краткое описание.
///@note Дополнительное пояснение.
///@param value описание параметра.
///@returns описание результата.
///@throws описание исключения.
```

- Новые комментарии должны быть содержательными, без пересказа очевидного кода.
- Для новых сущностей используются `#pragma once`, include через проектные пути и уже принятый порядок include-блоков.
- Имена файлов и каталогов должны соответствовать существующей структуре слоев.

## Namespace policy

Все новые серверные классы располагаются в namespace вида:

```cpp
namespace fasc::server::<название_слоя> {
}
```

Примеры серверных слоев:

- `fasc::server::core`
- `fasc::server::handling`
- `fasc::server::controllers::http`
- `fasc::server::controllers::app`
- `fasc::server::database`
- `fasc::server::persistence`
- `fasc::server::views`
- `fasc::server::marshalling`
- `fasc::server::security`

Вспомогательные классы общего назначения, которые не являются частью сервера, располагаются в:

```cpp
namespace fasc::extended::<группа_типов> {
}
```

Например, для result-подобных типов:

```cpp
namespace fasc::extended::fpp {
}
```

## JSON policy

Для ответов наружу используется схема `view -> json`.

Запрещено формировать HTTP-ответы через прямую сериализацию DTO:

```text
DTO -> json
```

Правильный путь:

```text
Persistence entity -> Result<DTO, Error> -> View/ErrorView -> JSON
```

DTO остаются внутренними структурами application boundary. View описывает публичный контракт API и не должен содержать чувствительные, технические или persistence-only поля.

Для каждого ресурса, который возвращается наружу, нужно создать view в `server/include/views/...` и `to_json` в `server/include/marshalling/...`.

## Схема базы данных

Источник схемы: `database/dump-mydb-sprinthost-public.sql`.

Таблицы из дампа:

| Таблица | Назначение |
| --- | --- |
| `farm_association` | фермерская ассоциация |
| `farm` | фермерское хозяйство |
| `association_farms` | связь хозяйств с ассоциациями |
| `person` | физическое лицо |
| `identity_document_type` | тип документа личности |
| `person_document` | документ физического лица |
| `farm_owner` | владелец хозяйства |
| `farm_ownership` | доля владения хозяйством |
| `farm_role` | роль сотрудника хозяйства |
| `association_role` | роль сотрудника ассоциации |
| `employment_status` | статус занятости |
| `farm_employee` | сотрудник хозяйства |
| `association_employee` | сотрудник ассоциации |
| `association_member` | член ассоциации |
| `farm_plot_type` | тип участка |
| `farm_plot` | участок |
| `farm_plot_assignment` | закрепление участка за хозяйством |
| `employee_plot_assignment` | закрепление участка за сотрудником |
| `unit` | единица измерения |
| `product_type` | тип продукции |
| `product` | продукт |
| `farm_plot_consumption_product` | потребление продукта на участке |
| `farm_plot_production_product` | производство продукта на участке |
| `supplier` | поставщик |
| `supplier_product_price` | цена продукта у поставщика |
| `contract` | договор |
| `purchase_requisition` | заявка хозяйства на закупку |
| `sales_requisition` | заявка хозяйства на продажу |
| `purchase_order` | заказ на закупку |
| `purchase_order_item` | позиция заказа на закупку |

Основные ключи:

- Простые `id` primary key есть у большинства справочников и сущностей.
- Составные primary key:
  - `association_farms(farm_id, association_id)`;
  - `farm_plot_assignment(farm_id, farm_plot_id)`;
  - `farm_plot_consumption_product(product_id, farm_plot_id)`;
  - `farm_plot_production_product(product_id, farm_plot_id)`.
- Уникальный ключ:
  - `product_type.sku`.

Основные связи:

- `farm_association` связан с `association_farms`, `association_employee`, `association_member`, `contract`, `purchase_order`.
- `farm` связан с `association_farms`, `farm_employee`, `farm_ownership`, `farm_plot_assignment`, `purchase_requisition`, `sales_requisition`, `contract`.
- `person` связан с `farm_owner`, `farm_employee`, `association_employee`, `association_member`, `person_document`, `purchase_order.created_by`.
- `farm_plot` связан с `farm_plot_type`, `farm_plot_assignment`, `employee_plot_assignment`, production/consumption таблицами.
- `product` связан с `product_type`, `unit`, requisitions, order items, supplier prices и production/consumption таблицами.
- `supplier` связан с `contract`, `purchase_order`, `supplier_product_price`.

## Группы серверных модулей

Реализацию нужно вести группами, чтобы не превращать таблицы в один большой контроллер.

Предлагаемые группы:

- `people`: `person`, `identity_document_type`, `person_document`.
- `associations`: `farm_association`, `association_role`, `association_employee`, `association_member`, `association_farms`.
- `farms`: `farm`, `farm_owner`, `farm_ownership`, `farm_role`, `farm_employee`, `employment_status`.
- `plots`: `farm_plot_type`, `farm_plot`, `farm_plot_assignment`, `employee_plot_assignment`.
- `products`: `unit`, `product_type`, `product`.
- `plot_products`: `farm_plot_consumption_product`, `farm_plot_production_product`.
- `suppliers`: `supplier`, `supplier_product_price`, `contract`.
- `procurement`: `purchase_requisition`, `purchase_order`, `purchase_order_item`.
- `sales`: `sales_requisition`.

Для каждой группы создаются одинаковые слои:

```text
server/include/persistence/<group>/...
server/include/views/<group>/...
server/include/marshalling/<group>/...
server/include/controllers/app/<group>/...
server/include/controllers/http/<group>/...
server/include/handling/<group>/...

server/src/controllers/app/<group>/...
server/src/controllers/http/<group>/...
server/src/handling/<group>/...
```

Если существующая сборка потребует более плоской структуры, допустимо сохранить текущий стиль каталогов, но namespace policy остается обязательной.

Текущая реализация использует плоскую структуру файлов по слоям: `server/include/persistence/Farm.hpp`, `server/include/views/Farm.hpp`, `server/src/controllers/app/Farm.cpp` и так далее. Группы выше остаются логической моделью предметной области, а не обязательной вложенностью каталогов.

## Persistence layer

Persistence-классы описывают таблицы из дампа. Для пользователя `server/include/persistence/User.hpp` используется полноценная ODB-генерация `user-odb.*` и `user.sql`. Для предметных CRUD-сущностей persistence-структуры зеркалят SQL-колонки и используются application layer вместе с универсальными методами `Database::selectRows`, `insertRow`, `updateRows`, `deleteRows`; отдельные ODB-generated файлы для каждой таблицы сейчас не собираются.

Требования:

- Имена таблиц и колонок должны соответствовать SQL-дампу.
- Типы PostgreSQL мапятся на C++ типы явно и предсказуемо:
  - `integer` -> `int` или проектный id type;
  - `smallint` -> `short`;
  - `numeric` -> `double` или отдельный decimal type, если он будет введен;
  - `character varying`, `text` -> `std::string`;
  - `date` -> выбранный проектный date type.
- Nullable-поля должны быть представлены через `std::optional<T>` или ODB-compatible nullable type.
- Для составных ключей нужно заранее выбрать единый подход: ODB composite value id или отдельный repository/query layer без искусственного изменения SQL-схемы.
- Схема из дампа является источником истины. Генерация новой ODB-схемы не должна незаметно менять существующую структуру базы.

## Application layer

Application controllers реализуют бизнес-сценарии и работают через `fasc::server::database::Database`.

Требования:

- HTTP, JSON и Beast-типы не попадают в application layer.
- Все операции записи выполняются внутри `Database::invokeTransactionally`.
- Методы application controller возвращают `fasc::extended::fpp::Result<SuccessType, ErrorType>`.
- `SuccessType` обычно является DTO/application result.
- `ErrorType` является предметной ошибкой конкретной группы, без HTTP-статуса и JSON-представления.
- Один application controller может использовать другой application controller через его `Result`, не завися от HTTP layer.

## HTTP layer

HTTP controllers отвечают за:

- разбор JSON-запроса в command DTO;
- вызов application controller;
- преобразование `Result<DTO, AppError>` в `Result<View, ErrorView>`;
- преобразование JSON parser errors в `ErrorView`;
- отсутствие формирования `HttpResponse`.

Handlers отвечают за:

- вызов HTTP controller;
- выбор HTTP-статуса по `View` или `ErrorView`;
- сериализацию `view -> json`;
- формирование итогового `HttpResponse`.

## API baseline

Реализованные служебные и пользовательские маршруты:

```text
GET  /health
POST /auth/register
POST /auth/login
POST /users
GET  /users
PUT  /users/role?login=<login>
```

Auth-запросы используют JSON с `login` и `password`. `/auth/register` создает пользователя с ролью `farm_worker`. Управление пользователями доступно ролям `agriculture_admin` и `association_director`: `POST /users` принимает `{ "login", "password", "role" }`, `GET /users` возвращает `{ "users": [...] }`, `PUT /users/role?login=<login>` принимает `{ "role": "<role>" }`. Все управленческие маршруты требуют `Authorization: Bearer <token>`. Успешный auth-ответ:

```json
{
  "token": "<jwt>",
  "token_type": "Bearer",
  "user": {
    "login": "alex",
    "role": "farm_worker"
  }
}
```

При старте сервер обеспечивает встроенного администратора `admin` / `admin12345` с ролью `agriculture_admin`. Значения можно переопределить через `FARM_ADMIN_LOGIN` и `FARM_ADMIN_PASSWORD`, а создание отключить через `FARM_ADMIN_ENABLED=0`.

Для каждой предметной таблицы зарегистрирован набор маршрутов:

```text
GET    /api/<resource>
POST   /api/<resource>
GET    /api/<resource>/item?<key>
PUT    /api/<resource>/item?<key>
DELETE /api/<resource>/item?<key>
```

Для простых ключей `<key>` обычно равен `id=<id>`. Для составных ключей используются query parameters всех колонок ключа:

```text
association_farms: farm_id=<id>&association_id=<id>
farm_plot_assignment: farm_id=<id>&farm_plot_id=<id>
farm_plot_consumption_product: product_id=<id>&farm_plot_id=<id>
farm_plot_production_product: product_id=<id>&farm_plot_id=<id>
```

Списки возвращаются как `{ "resource": "<resource>", "rows": [...] }`, одиночная строка как `{ "resource": "<resource>", "data": { ... } }`, результат записи как `{ "resource": "<resource>", "affectedRows": <number> }`.

## Ошибки API

Ошибки возвращаются в JSON:

```json
{
  "error": "message"
}
```

Базовое соответствие:

- `400 Bad Request` - неверный JSON, неверные типы, validation error.
- `401 Unauthorized` - ошибка авторизации.
- `403 Forbidden` - JWT валиден, но роль не имеет права на операцию.
- `404 Not Found` - сущность или маршрут не найдены.
- `409 Conflict` - конфликт уникальности или состояния.
- `500 Internal Server Error` - непредвиденная ошибка сервера.

## Сборка

Нужно развивать текущий `server/CMakeLists.txt`.

Требования:

- новые `.cpp` добавляются в `farm_association_server_core`;
- новые ODB-generated файлы добавляются через CMake, а не вручную;
- `server/third_party` остается единственным локальным источником зависимостей;
- тесты подключаются через существующий `server/tests/CMakeLists.txt`.

## Тестирование

Для каждой группы таблиц нужны:

- unit-тесты marshalling `view -> json`;
- unit-тесты HTTP controller на преобразование application result в view/error view;
- unit-тесты handler на статусы и тела ответов;
- unit-тесты application controller на `Result<SuccessType, ErrorType>`;
- integration-тесты endpoint boundary по существующему стилю;
- по возможности business process tests для связанных сценариев.

Минимальная проверка перед завершением этапа:

```powershell
cmake --build server\build
ctest --test-dir server\build --output-on-failure
```

Если используется другой build directory, команды адаптируются к нему.

## Порядок реализации

Текущий этап уже содержит общий CRUD-каркас и маршруты для всех таблиц предметной области из дампа. Клиентский auth-контракт синхронизирован с серверным `login`/`role`, а управление пользователями защищено JWT ролей `agriculture_admin` и `association_director`. Дальнейший порядок работ:

1. Добавить проверку `Authorization: Bearer <token>` и server-side RBAC для предметных `/api/*` маршрутов.
2. Добавить endpoint `/auth/me` для восстановления сессии по JWT.
3. Расширить бизнес-сценарии поверх базового CRUD: отчеты, согласования, фильтры и связанные операции.
4. Поддерживать тесты и документацию при изменении API, сборки или схемы БД.

## Критерии готовности

Сервер считается готовым для текущего этапа, когда:

- persistence-модель соответствует таблицам из `database/dump-mydb-sprinthost-public.sql`;
- новые классы разложены по namespace согласно спецификации;
- публичные ответы строятся через `view -> json`;
- код документирован в существующем стиле;
- проект собирается через CMake с локальными зависимостями из `server/third_party`;
- тесты проходят или явно описаны причины, если часть integration-тестов требует внешней PostgreSQL базы.
