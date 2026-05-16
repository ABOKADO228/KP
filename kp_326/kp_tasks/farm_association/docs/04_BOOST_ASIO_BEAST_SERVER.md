# 04. Boost.Asio И Boost.Beast В Сервере

Документ описывает текущий HTTP-слой проекта: асинхронный Boost.Beast server, request dispatcher, routing и границу между transport layer и application layer.

## Роли Библиотек

Boost.Asio отвечает за сетевой уровень:

```text
io_context
tcp::acceptor
tcp::socket
signal_set
```

Boost.Beast отвечает за HTTP поверх TCP:

```text
http::request_parser
http::response
http::async_read
http::async_write
flat_buffer
tcp_stream
```

Application layer видит не Beast-типы, а проектные `HttpRequest` и `HttpResponse`.

## Основной Путь Запроса

```text
TCP client
  -> Listener::async_accept
  -> HttpSession
  -> http::async_read
  -> BeastRequest
  -> RequestDispatcher
  -> HttpRequest
  -> AppRouter
  -> Handler
  -> Controller
  -> Database wrapper
  -> HttpResponse
  -> BeastResponse
  -> http::async_write
  -> TCP client
```

Для `GET /health` цепочка короче: handler возвращает JSON без обращения к базе.

## Server

Файлы:

```text
server/include/server/core/Server.hpp
server/src/server/core/Server.cpp
```

Публичный API:

```cpp
void get(std::string path, AppRouter::RouteHandler handler);
void post(std::string path, AppRouter::RouteHandler handler);
void put(std::string path, AppRouter::RouteHandler handler);
void del(std::string path, AppRouter::RouteHandler handler);
int run(unsigned short port);
int run(unsigned short port, std::size_t thread_count);
int run(const ServerSettings& settings);
```

`run(const ServerSettings&)` является основным entrypoint. Старые overloads оставлены как удобные сокращения.

## ServerSettings

```cpp
struct ServerSettings {
  std::string address{"0.0.0.0"};
  unsigned short port{8080};
  std::size_t thread_count{0};
  std::size_t request_body_limit{1024 * 1024};
  std::chrono::seconds request_timeout{30};
  int listen_backlog{0};
};
```

`thread_count = 0` означает auto-detect через `std::thread::hardware_concurrency()`. `listen_backlog = 0` означает системное значение Boost.Asio.

## Listener

`Listener` владеет `tcp::acceptor` и принимает соединения:

```cpp
acceptor_.async_accept(
    beast::bind_front_handler(&Listener::onAccept, shared_from_this()));
```

После успешного accept создается отдельная session:

```cpp
std::make_shared<HttpSession>(std::move(socket), dispatcher_, settings_)->run();
```

Listener не обрабатывает HTTP сам. Его зона ответственности только accept loop.

## HttpSession

`HttpSession` представляет одно TCP-соединение. Она хранит:

```text
beast::tcp_stream
beast::flat_buffer
request_parser<string_body>
shared response
RequestDispatcher reference
ServerSettings reference
```

Session читает запрос через parser:

```cpp
parser_ = std::make_unique<RequestParser>();
parser_->body_limit(settings_.request_body_limit);
stream_.expires_after(settings_.request_timeout);
http::async_read(stream_, buffer_, *parser_, ...);
```

После успешного чтения request dispatch-ится и записывается обратно:

```cpp
response_ = std::make_shared<BeastResponse>(dispatcher_.dispatch(parser_->get()));
http::async_write(stream_, *response_, ...);
```

Если клиент запросил keep-alive и запись прошла успешно, session читает следующий запрос на том же соединении.

## RequestDispatcher

Файлы:

```text
server/include/server/core/RequestDispatcher.hpp
server/src/server/core/RequestDispatcher.cpp
```

Dispatcher преобразует:

```text
BeastRequest -> HttpRequest
HttpResponse -> BeastResponse
```

Он нормализует target: path идет в `HttpRequest::target`, query string разбирается в `HttpRequest::query_params`.

```text
/api/farm/1?verbose=true
  target: /api/farm/1
  query_params: verbose=true
```

Dispatcher также добавляет в ответ:

```text
Server: farm-association-server
X-Content-Type-Options: nosniff
Content-Type: <application response content type>
```

## AppRouter

Файлы:

```text
server/include/server/core/AppRouter.hpp
server/src/server/core/AppRouter.cpp
```

Router хранит обработчики по ключу `METHOD path`. Примеры:

```text
GET /health
POST /auth/register
POST /auth/login
POST /users
GET /api/farm
POST /api/farm
GET /api/farm/item
PUT /api/farm/item
DELETE /api/farm/item
```

`POST /users` проходит через тот же router, но handler дополнительно проверяет `Authorization: Bearer <token>` и допускает только роль `agriculture_admin`.

Query string не входит в ключ маршрута: `GET /api/farm/item?id=1` ищется как `GET /api/farm/item`, а `id=1` попадает в `HttpRequest::query_params`.

Если маршрут не найден, возвращается `404` с JSON-ошибкой. Если handler бросил исключение, router превращает его в `500` с JSON-ошибкой.

## Ошибки Протокола

Если Beast parser вернул ошибку, session отвечает JSON-ошибкой на transport layer:

```text
body_limit exceeded -> 413 Payload Too Large
other parser error  -> 400 Bad Request
```

Такие ошибки не проходят через application handlers, потому что корректный `HttpRequest` еще не создан.

## Потоки И Shutdown

`Server::run` создает `io_context`, запускает `Listener`, регистрирует `SIGINT`/`SIGTERM` и поднимает worker threads:

```cpp
for (std::size_t i = 0; i < worker_count; ++i) {
  workers.emplace_back([&io] { io.run(); });
}
```

Если worker поймал необработанное исключение, сервер останавливает `io_context`, join-ит потоки и пробрасывает исключение наружу.

## Как Читать Код

Рекомендуемый порядок:

```text
server/src/Main.cpp
server/include/server/core/Server.hpp
server/src/server/core/Server.cpp
server/include/server/core/RequestDispatcher.hpp
server/src/server/core/RequestDispatcher.cpp
server/include/server/core/AppRouter.hpp
server/src/server/core/AppRouter.cpp
server/src/handling/Health.cpp
server/src/handling/Users.cpp
server/src/handling/FarmEntityRoutes.cpp
```
