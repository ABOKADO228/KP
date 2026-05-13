# 06. Reproduced database and CRUD API

В корень проекта добавлен SQL-дамп:

```text
database/dump-mydb-sprinthost-public.sql
```

Он взят из исходного файла:

```text
D:\Development\Projects\KP\kp_326\data\dump\dump-mydb-sprinthost-public.sql
```

## Как воспроизвести базу

Создать пустую базу и применить дамп:

```powershell
createdb -h localhost -p 5432 -U postgres farm_association
psql -h localhost -p 5432 -U postgres -d farm_association -f database\dump-mydb-sprinthost-public.sql
```

Переменные окружения сервера должны указывать на эту базу:

```powershell
$env:FARM_DB_HOST="localhost"
$env:FARM_DB_PORT="5432"
$env:FARM_DB_NAME="farm_association"
$env:FARM_DB_USER="postgres"
$env:FARM_DB_PASSWORD="password"
```

## CRUD endpoints

Для всех таблиц из дампа зарегистрированы маршруты:

```text
GET    /api/{table}
POST   /api/{table}
PUT    /api/{table}
DELETE /api/{table}
```

Список таблиц зафиксирован в `server/src/crud/CrudSchema.cpp`. Имена таблиц и колонок не принимаются произвольно из HTTP-запроса: сервер проверяет их по registry. Значения передаются в PostgreSQL через `PQexecParams`.

## Примеры

Список ферм:

```http
GET /api/farm?limit=50&offset=0
```

Ответ:

```json
{
  "items": [],
  "limit": 50,
  "offset": 0
}
```

Создать запись:

```http
POST /api/unit
Content-Type: application/json

{
  "id": 100,
  "code": "KG",
  "name": "Kilogram"
}
```

Обновить запись с простым primary key:

```http
PUT /api/unit?id=100
Content-Type: application/json

{
  "name": "Kilogram updated"
}
```

Удалить запись с составным primary key:

```http
DELETE /api/association_farms?farm_id=1&association_id=2
```

## Возвраты

Успешные ответы:

```text
GET    200 {"items":[...],"limit":100,"offset":0}
POST   201 {record}
PUT    200 {record}
DELETE 200 {"deleted":{record}}
```

Ошибки:

```text
400 некорректный JSON, неизвестная колонка, отсутствует primary key
404 неизвестная таблица или запись не найдена
500 ошибка PostgreSQL
```

## OpenAPI

Файл `openapi.yaml` в корне проекта описывает auth endpoints и общий CRUD endpoint `/api/{table}`. Его можно открыть в Swagger Editor или Swagger UI.
