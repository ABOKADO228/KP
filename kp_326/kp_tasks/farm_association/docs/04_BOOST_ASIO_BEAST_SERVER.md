# 04. Boost.Asio и Boost.Beast в сервере

Документ объясняет, как работает текущий HTTP-сервер, какую роль играют Boost.Asio и Boost.Beast, и как запрос проходит от TCP-сокета до application handler.

## Разделение ролей

Boost.Asio отвечает за сетевой уровень:

```text
io_context
tcp::acceptor
tcp::socket
```

Boost.Beast отвечает за HTTP поверх TCP:

```text
http::request
http::response
http::read
http::write
flat_buffer
```

В проекте это обернуто в свои классы:

```text
Server
RequestDispatcher
AppRouter
HealthHandler
UserHandler
UserHttpController
UserController
```

## Общий путь запроса

Для `GET /health` путь такой:

```text
TCP client
  -> boost::asio::ip::tcp::acceptor
  -> boost::asio::ip::tcp::socket
  -> boost::beast::http::read
  -> BeastRequest
  -> RequestDispatcher
  -> HttpRequest
  -> AppRouter
  -> HealthHandler
  -> HttpResponse
  -> BeastResponse
  -> boost::beast::http::write
  -> TCP client
```

Для `POST /users` путь длиннее:

```text
RequestDispatcher
  -> AppRouter
  -> UserHandler
  -> UserHttpController
  -> JSON parsing
  -> UserController
  -> Database
  -> ODB
  -> PostgreSQL
```

## Server

Файл:

```text
server/include/server/core/Server.hpp
server/src/server/core/Server.cpp
```

`Server` хранит `AppRouter` и дает методы:

```cpp
void get(std::string path, AppRouter::RouteHandler handler);
void post(std::string path, AppRouter::RouteHandler handler);
int run(unsigned short port);
```

`get` и `post` регистрируют маршруты. `run` открывает сетевой порт и начинает принимать соединения.

## io_context

В `Server::run` создается:

```cpp
boost::asio::io_context io;
```

`io_context` - центральный объект Asio. Через него работают асинхронные операции, timers, sockets и acceptors.

В текущем сервере код синхронный, но `io_context` все равно нужен, потому что Asio socket/acceptor привязаны к execution context.

## tcp::acceptor

```cpp
tcp::acceptor acceptor{io, {tcp::v4(), port}};
```

Синтаксис:

```text
tcp::acceptor
  объект, который слушает входящие TCP-соединения

io
  io_context, к которому привязан acceptor

{tcp::v4(), port}
  endpoint: IPv4 + номер порта
```

Если порт занят, создание `acceptor` выбросит исключение.

## tcp::socket

В цикле:

```cpp
tcp::socket socket{io};
acceptor.accept(socket);
```

Смысл:

```text
tcp::socket socket{io}
  создать пустой socket

acceptor.accept(socket)
  заблокировать текущий поток до входящего соединения
  и поместить принятое соединение в socket
```

Текущая модель сервера обслуживает соединения последовательно. Это проще для обучения, но не является высокопроизводительной многопоточной архитектурой.

## Beast buffer

```cpp
boost::beast::flat_buffer buffer;
```

`flat_buffer` нужен Beast-у как промежуточное хранилище байтов, прочитанных из TCP stream. HTTP parser может получить не весь запрос сразу, поэтому ему нужен buffer между чтениями.

## BeastRequest

В проекте:

```cpp
using BeastRequest = boost::beast::http::request<boost::beast::http::string_body>;
```

`http::request<string_body>` означает:

```text
http::request
  HTTP request type

string_body
  тело запроса хранится как std::string
```

Чтение:

```cpp
http::read(socket, buffer, request);
```

Смысл:

```text
socket
  откуда читать bytes

buffer
  временное хранилище для parser-а

request
  объект, куда Beast положит method, target, headers, body
```

## RequestDispatcher

Файлы:

```text
server/include/server/core/RequestDispatcher.hpp
server/src/server/core/RequestDispatcher.cpp
```

Задача dispatcher-а - отделить Boost.Beast от application layer.

Он преобразует:

```text
BeastRequest -> HttpRequest
HttpResponse -> BeastResponse
```

Так application code не зависит напрямую от Boost.Beast types.

Пример:

```cpp
app_request.method = std::string(request.method_string());
app_request.target = std::string(request.target());
app_request.body = request.body();
```

Здесь Beast-specific данные переводятся в простую структуру `HttpRequest`.

## AppRouter

Файлы:

```text
server/include/server/core/AppRouter.hpp
server/src/server/core/AppRouter.cpp
```

Router хранит map:

```cpp
std::unordered_map<std::string, RouteHandler> routes_;
```

Ключ строится из method и path:

```cpp
std::string AppRouter::key(std::string method, std::string path) {
  return std::move(method) + " " + std::move(path);
}
```

Например:

```text
GET /health
POST /users
```

Если маршрут не найден, router возвращает:

```json
{"error":"not found"}
```

со статусом `404`.

Если handler выбросил исключение, router возвращает:

```json
{"error":"..."}
```

со статусом `500`.

## Handlers и controllers

`HealthHandler`:

```text
handling/Health.hpp
handling/Health.cpp
```

Возвращает простой ответ:

```json
{"status":"ok"}
```

`UserHandler`:

```text
handling/Users.hpp
handling/Users.cpp
```

Передает запрос в `UserHttpController`.

`UserHttpController`:

```text
controllers/http/Users.hpp
controllers/http/Users.cpp
```

Отвечает за HTTP/JSON:

```text
JSON body -> CreateUserCommand
UserDto -> UserView -> JSON body
HTTP status 201
```

`UserController`:

```text
controllers/app/Users.hpp
controllers/app/Users.cpp
```

Отвечает за бизнес-логику:

```text
проверить name
создать persistence::User
сохранить через Database
вернуть UserDto
```

## Почему нужен свой HttpRequest

Можно было бы передавать `boost::beast::http::request` во все handlers. Но тогда весь application layer зависел бы от Beast.

Сейчас граница такая:

```text
Boost.Beast
  только transport/dispatcher layer

HttpRequest/HttpResponse
  простой контракт внутри приложения

controllers/handlers
  не обязаны знать детали Beast
```

Это делает тесты проще: можно создать `HttpRequest` без настоящего socket-а.

## Текущие ограничения

Сервер сейчас синхронный:

```text
accept one connection
read request
route request
write response
go to next connection
```

Это хорошо для начального этапа, потому что код легко читать.

Для высокопроизводительного сервера следующий шаг:

```text
async_accept
async_read
async_write
io_context.run() в пуле потоков
strand для сериализации доступа к session state
connection/session object на каждое соединение
```

## Как читать код

Рекомендуемый порядок:

```text
1. server/src/main.cpp
2. server/include/server/core/Server.hpp
3. server/src/server/core/Server.cpp
4. server/include/server/core/RequestDispatcher.hpp
5. server/src/server/core/RequestDispatcher.cpp
6. server/include/server/core/AppRouter.hpp
7. server/src/server/core/AppRouter.cpp
8. server/include/handling/Health.hpp
9. server/include/handling/Users.hpp
10. server/include/controllers/http/Users.hpp
11. server/include/controllers/app/Users.hpp
```

Так ты увидишь путь запроса сверху вниз: от `main()` до бизнес-логики.
## Синхронная модель текущего сервера

Текущий `Server::run` можно представить как псевдокод:

```cpp
for (;;) {
  tcp::socket socket{io};
  acceptor.accept(socket);

  boost::beast::flat_buffer buffer;
  BeastRequest request;
  http::read(socket, buffer, request);

  BeastResponse response = dispatcher.dispatch(request);
  http::write(socket, response);
}
```

Ключевое свойство: каждая операция блокирует текущий поток.

```text
accept
  ждет новое TCP-соединение

read
  ждет HTTP request

dispatch
  выполняет application logic

write
  отправляет HTTP response
```

Пока один клиент медленно отправляет запрос, сервер не принимает следующий. Это нормально для учебного сервера, но плохо для production-нагрузки.

## Blocking I/O

Blocking I/O означает: вызов функции не возвращает управление, пока операция не завершится.

Пример:

```cpp
acceptor.accept(socket);
```

Пока клиент не подключится, выполнение стоит на этой строке.

Пример:

```cpp
http::read(socket, buffer, request);
```

Пока Beast не прочитает полный HTTP request, выполнение стоит на этой строке.

Плюсы:

```text
код легко читать
меньше callback/state machine
удобно изучать протокол
```

Минусы:

```text
один поток обслуживает только одну операцию за раз
медленный клиент тормозит остальных
сложно масштабировать под много соединений
```

## Как выглядит async-модель

Асинхронный сервер строится иначе:

```text
async_accept
  зарегистрировать операцию принятия соединения

async_read
  зарегистрировать операцию чтения запроса

async_write
  зарегистрировать операцию записи ответа

io_context.run()
  выполнять готовые handlers
```

В async-коде функция не ждет завершения операции. Она говорит Asio: "начни операцию и вызови мой handler, когда будет результат".

Примерный вид:

```cpp
acceptor.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        start_session(std::move(socket));
      }
      accept_next();
    });
```

Смысл параметров lambda:

```text
boost::system::error_code ec
  ошибка операции; если ec пустой, операция успешна

tcp::socket socket
  принятое соединение
```

## io_context.run()

В синхронном коде `io_context.run()` явно не нужен, потому что используются blocking functions.

В async-коде он обязателен:

```cpp
io.run();
```

`run()` запускает event loop. Он берет готовые completion handlers и выполняет их.

Если запустить несколько потоков:

```cpp
std::vector<std::thread> threads;
for (std::size_t i = 0; i < thread_count; ++i) {
  threads.emplace_back([&] {
    io.run();
  });
}
```

то несколько потоков будут вместе обслуживать готовые сетевые события.

## Session object

В async Beast server обычно создают объект session на каждое соединение.

Он хранит:

```text
tcp::socket
flat_buffer
BeastRequest
BeastResponse
RequestDispatcher reference
```

Зачем хранить это в объекте:

```text
async operation завершается позже
локальные переменные функции к тому моменту уже исчезнут
session должен жить до завершения read/write
```

Типичный паттерн:

```cpp
class Session : public std::enable_shared_from_this<Session> {
public:
  void run();
private:
  void read();
  void write();
};
```

`shared_from_this()` нужен, чтобы session не уничтожилась, пока идет async operation.

## Strand

Если один connection/session может получать callbacks из разных потоков, нужен способ не выполнять два handler-а одной session одновременно.

Для этого Asio использует strand:

```text
strand
  serial executor; гарантирует последовательное выполнение handlers,
  привязанных к нему
```

Без strand можно получить гонки:

```text
thread A выполняет read handler
thread B одновременно выполняет write handler
оба меняют состояние session
```

С strand handlers одной session выполняются последовательно, даже если `io_context.run()` работает в нескольких потоках.

## HTTP request anatomy

HTTP request состоит из:

```text
method
target
version
headers
body
```

Пример:

```http
POST /users HTTP/1.1
Host: localhost:8080
Content-Type: application/json
Content-Length: 15

{"name":"Alex"}
```

В Beast:

```cpp
request.method_string();
request.target();
request.version();
request.body();
```

Headers обходятся так:

```cpp
for (const auto& field : request) {
  app_request.headers.emplace(
      std::string(field.name_string()),
      std::string(field.value()));
}
```

`field.name_string()` - имя header-а.

`field.value()` - значение header-а.

## HTTP response anatomy

HTTP response состоит из:

```text
status
version
headers
body
```

В проекте:

```cpp
BeastResponse response{static_cast<http::status>(app_response.status), request.version()};
response.set(http::field::server, "farm-association-server");
response.set(http::field::content_type, app_response.content_type);
response.keep_alive(request.keep_alive());
response.body() = app_response.body;
response.prepare_payload();
```

Разбор:

```text
static_cast<http::status>
  преобразовать unsigned status в enum Beast status

request.version()
  ответ использует ту же HTTP version, что запрос

response.set(...)
  добавить header

keep_alive(...)
  сохранить connection policy клиента

body() = ...
  записать response body

prepare_payload()
  пересчитать Content-Length и подготовить сообщение к отправке
```

Если забыть `prepare_payload()`, response может уйти без корректного `Content-Length`.

## Где заканчивается HTTP и начинается бизнес-логика

Граница проходит здесь:

```text
UserHttpController
  HTTP + JSON + status code

UserController
  business rules + database operations
```

Например:

```cpp
auto command = nlohmann::json::parse(request.body).get<CreateUserCommand>();
```

Это HTTP/controller layer, потому что он читает JSON body.

А вот:

```cpp
if (command.name.empty()) {
  throw std::invalid_argument("User name is required");
}
```

Это business rule, поэтому оно находится в application controller.

## Как тестируются endpoints

Integration test не открывает настоящий TCP socket. Он создает Beast request напрямую:

```cpp
const BeastResponse response =
    dispatcher.dispatch(makeBeastRequest(http::verb::get, "/health"));
```

Это проверяет цепочку:

```text
BeastRequest
  -> RequestDispatcher
  -> AppRouter
  -> HealthHandler
  -> BeastResponse
```

Плюсы такого теста:

```text
быстрый
не требует свободного порта
не требует настоящего network stack
проверяет HTTP mapping и routing
```

Минусы:

```text
не проверяет tcp::acceptor
не проверяет реальный socket
не проверяет конкуренцию клиентов
```

Для полного end-to-end теста позже можно запускать server process на тестовом порту и отправлять настоящий HTTP request.

## Как развивать сервер дальше

Рекомендуемый путь:

```text
1. Оставить текущий синхронный сервер как baseline.
2. Вынести session logic в отдельный класс.
3. Добавить async_accept.
4. Добавить async_read/async_write.
5. Добавить thread pool над io_context.run().
6. Добавить graceful shutdown.
7. Добавить request timeout.
8. Добавить лимит размера body.
9. Добавить structured error responses.
10. Добавить реальные DB integration tests.
```

Каждый шаг лучше покрывать тестами отдельно. Иначе легко смешать сетевые ошибки, routing bugs и database bugs в одну трудно отлаживаемую проблему.

## Мини-упражнения

1. Добавить endpoint `GET /version`, который возвращает имя и версию сервера.
2. Добавить unit test для `AppRouter`, проверяющий, что `POST /health` не совпадает с `GET /health`.
3. Добавить integration test для неизвестного endpoint: `GET /unknown -> 404`.
4. Добавить проверку `Content-Type` в `UserHttpController`.
5. Добавить business rule: имя пользователя должно быть не короче 2 символов.

Эти упражнения идут от простого transport/routing к business validation.
