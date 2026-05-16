# 05. Production-ready Boost.Beast Server Layer

Документ фиксирует текущее состояние HTTP server layer: асинхронный многопоточный I/O, явная конфигурация, лимиты, корректные protocol errors, безопасный JSON для ошибок и аккуратная граница между transport layer и application layer.

## Что Реализовано

Ключевые файлы:

```text
server/include/server/core/Server.hpp
server/src/server/core/Server.cpp
server/include/server/core/RequestDispatcher.hpp
server/src/server/core/RequestDispatcher.cpp
server/src/server/core/AppRouter.cpp
server/src/Main.cpp
```

Текущее поведение:

```text
async_accept
http::async_read
http::async_write
HttpSession per TCP connection
worker thread pool over one io_context
keep-alive request loop
request body limit
request timeout
SIGINT/SIGTERM shutdown
query string parsing
JSON protocol errors
X-Content-Type-Options: nosniff
```

## Конфигурация Запуска

`main()` читает HTTP-настройки из окружения:

```text
FARM_SERVER_ADDRESS
FARM_SERVER_PORT
FARM_SERVER_THREADS
FARM_SERVER_BODY_LIMIT
FARM_SERVER_TIMEOUT_SECONDS
FARM_SERVER_LISTEN_BACKLOG
```

CLI-аргументы применяются после env и переопределяют их:

```powershell
.\server\build\farm_association_server.exe --address 127.0.0.1 --port 8080 --threads 8 --body-limit 1048576 --timeout 30 --backlog 1024
```

Проверка версии:

```powershell
.\server\build\farm_association_server.exe --version
```

## Runtime Database Bootstrap

Перед запуском HTTP loop `main()` готовит PostgreSQL:

```text
Debug
  uses fasc_test
  drops and recreates it on every start

Release
  uses fasc_db
  creates and initializes missing schemas
```

Затем создается `Database` wrapper и регистрируются routes. Если PostgreSQL недоступен, сервер завершится до открытия HTTP-порта, что лучше, чем принимать запросы в неготовом состоянии.

## HttpSession

Session читает запрос parser-ом, чтобы задать лимит тела:

```cpp
parser_ = std::make_unique<RequestParser>();
parser_->body_limit(settings_.request_body_limit);
stream_.expires_after(settings_.request_timeout);
http::async_read(stream_, buffer_, *parser_, ...);
```

Ответ хранится в `std::shared_ptr`, потому что `async_write` завершится позже, чем функция, которая его запустила:

```cpp
response_ = std::make_shared<BeastResponse>(dispatcher_.dispatch(parser_->get()));
http::async_write(stream_, *response_, ...);
```

После успешной записи keep-alive соединение возвращается к чтению следующего запроса. Без keep-alive session закрывает write side socket-а.

## Protocol Errors

Ошибки Beast parser-а превращаются в JSON:

```text
http::error::body_limit -> 413 Payload Too Large
other parser errors     -> 400 Bad Request
```

Формирование JSON идет через `nlohmann::json`, поэтому текст ошибки безопасно экранируется.

## Dispatcher Boundary

`RequestDispatcher` держит `const AppRouter&`, не мутирует routes во время работы сервера и переводит Beast-типы в проектные DTO:

```text
BeastRequest -> HttpRequest
HttpResponse -> BeastResponse
```

Query string не участвует в поиске маршрута:

```text
/health?verbose=true -> route /health, query verbose=true
```

## Потоковая Модель

`Server::run` нормализует число потоков:

```text
thread_count = 0 -> hardware_concurrency или 1
thread_count > 0 -> заданное значение, минимум 1
```

Все workers вызывают `io.run()`. Если один поток ловит необработанное исключение, `io.stop()` останавливает сервер, все threads join-ятся, и исключение пробрасывается наружу.

## Что Проверять При Изменениях

Минимальный цикл:

```powershell
cmake --build server\build --config Debug --parallel
$pg = "$PWD\server\third_party\postgresql"
if (!(Test-Path "$pg\data\PG_VERSION")) { & "$pg\bin\initdb.exe" -D "$pg\data" -U postgres -A trust --encoding=UTF8 --locale=C }
& "$pg\bin\pg_isready.exe" -h localhost -p 5432
if ($LASTEXITCODE -ne 0) { & "$pg\bin\pg_ctl.exe" -D "$pg\data" -l "$pg\postgres.log" start }
ctest --test-dir server\build -C Debug --output-on-failure
```

Полный цикл:

```powershell
cmake --build server\build --config Debug --parallel
ctest --test-dir server\build -C Debug --output-on-failure
cmake --build server\build-release --config Release --parallel
ctest --test-dir server\build-release -C Release --output-on-failure
```

CTest уже включает DB-backed integration tests, поэтому PostgreSQL должен быть запущен. После этого можно поднять сервер и проверить:

```bash
curl http://localhost:8080/health
curl http://localhost:8080/health?verbose=true
curl -X POST http://localhost:8080/auth/register -H "Content-Type: application/json" -d '{"login":"alex","password":"password123"}'
curl http://localhost:8080/api/farm/item?id=1
```

## Следующие Улучшения

Полезные направления, если понадобится усилять HTTP layer дальше:

```text
structured request logging
graceful drain before shutdown
real DB-backed end-to-end smoke tests
rate limiting
TLS termination or reverse-proxy documentation
request id propagation
```
