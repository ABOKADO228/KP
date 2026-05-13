# 05. Production-ready Boost.Beast server layer

Документ описывает, как текущий серверный слой был усилен до более production-подхода: асинхронный многопоточный I/O, явная конфигурация, лимиты, корректные HTTP-ошибки, безопасная сериализация JSON и более строгая граница между transport layer и application layer.

## Что было до

Изначально `Server::run` работал как последовательный блокирующий цикл:

```cpp
for (;;) {
  tcp::socket socket{io};
  acceptor.accept(socket);
  http::read(socket, buffer, request);
  BeastResponse response = dispatcher.dispatch(request);
  http::write(socket, response);
}
```

Семантика: сервер принимает одно соединение, читает один запрос, обрабатывает его, отправляет ответ и только потом переходит к следующему клиенту. Это просто, но плохо для нагрузки: медленный клиент блокирует весь accept/read/write цикл.

## Что сделано

Измененные файлы:

```text
server/include/server/core/server.hpp
server/src/server/core/server.cpp
server/include/server/core/RequestDispatcher.hpp
server/src/server/core/RequestDispatcher.cpp
server/src/server/core/AppRouter.cpp
server/src/main.cpp
```

Ключевые изменения:

1. Blocking I/O заменен на `async_accept`, `http::async_read`, `http::async_write`.
2. Каждое соединение обслуживает отдельный объект `HttpSession`.
3. `io_context.run()` запускается в пуле worker-потоков.
4. Добавлен `ServerSettings` для адреса, порта, потоков, body limit, timeout и backlog.
5. Добавлен `http::request_parser<string_body>` с `body_limit`.
6. Protocol-level ошибки возвращают JSON-ответы: `400 Bad Request` или `413 Payload Too Large`.
7. Поддержан keep-alive цикл: одно TCP-соединение может обработать несколько HTTP-запросов.
8. Добавлен shutdown по `SIGINT` и `SIGTERM`.
9. `RequestDispatcher` хранит `const AppRouter&`, то есть не мутирует роутер во время работы.
10. Query string больше не участвует в поиске маршрута: `/health?verbose=true` маршрутизируется как `/health`.
11. JSON ошибок собирается через `nlohmann::json`, а не ручной конкатенацией.
12. В ответы добавлен `X-Content-Type-Options: nosniff`.
13. `main()` читает настройки из env и CLI, а не держит `8080` жестко в коде.

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

Семантика:

```text
address
  IP-адрес bind-а. 0.0.0.0 означает все IPv4-интерфейсы.

port
  TCP-порт сервера.

thread_count
  Количество worker-потоков. 0 означает auto через hardware_concurrency().

request_body_limit
  Максимальный размер HTTP body в байтах.

request_timeout
  Максимальное время ожидания сетевой операции в session.

listen_backlog
  Размер очереди входящих TCP-соединений. 0 означает системное значение Boost.Asio.
```

Публичный API:

```cpp
int run(unsigned short port);
int run(unsigned short port, std::size_t thread_count);
int run(const ServerSettings& settings);
```

Старые overloads сохранены для совместимости, но основной production-вход теперь `run(settings)`.

## Конфигурация запуска

`main()` читает настройки из переменных окружения:

```text
FARM_SERVER_ADDRESS
FARM_SERVER_PORT
FARM_SERVER_THREADS
FARM_SERVER_BODY_LIMIT
FARM_SERVER_TIMEOUT_SECONDS
FARM_SERVER_LISTEN_BACKLOG
```

И из CLI:

```powershell
.\server\build\farm_association_server.exe --address 127.0.0.1 --port 8080 --threads 8 --body-limit 1048576 --timeout 30 --backlog 1024
```

CLI применяется после env и переопределяет env. Это удобно для контейнеров, CI, systemd и локального запуска.

## HttpSession

`HttpSession` представляет одно TCP-соединение:

```cpp
class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
  HttpSession(tcp::socket socket,
              const RequestDispatcher& dispatcher,
              const ServerSettings& settings);
  void run();
};
```

`std::enable_shared_from_this` нужен, потому что async-операция завершится позже. Callback должен удерживать session живой:

```cpp
http::async_read(stream_, buffer_, *parser_,
                 beast::bind_front_handler(&HttpSession::on_read, shared_from_this()));
```

Семантика:

```text
shared_from_this()
  Передает callback-у shared_ptr на текущую session.

bind_front_handler(...)
  Привязывает метод класса и объект session к completion handler.

async_read(...)
  Запускает чтение и сразу возвращает управление event loop.
```

## Parser и лимит тела

Вместо чтения напрямую в `BeastRequest` используется parser:

```cpp
parser_ = std::make_unique<RequestParser>();
parser_->body_limit(settings_.request_body_limit);

stream_.expires_after(settings_.request_timeout);
http::async_read(stream_, buffer_, *parser_,
                 beast::bind_front_handler(&HttpSession::on_read, shared_from_this()));
```

Семантика:

```text
RequestParser
  HTTP parser Boost.Beast для request<string_body>.

body_limit(...)
  Ограничивает максимальный размер body и защищает память сервера.

expires_after(...)
  Ставит timeout на операцию чтения/записи.
```

После успешного чтения request берется из parser-а:

```cpp
response_ = std::make_shared<BeastResponse>(dispatcher_.dispatch(parser_->get()));
```

`response_` хранится в `shared_ptr`, потому что объект ответа должен жить до завершения `async_write`.

## Ошибки протокола

Если Beast parser вернул ошибку, session формирует HTTP-ответ:

```cpp
const auto status = error == http::error::body_limit
    ? http::status::payload_too_large
    : http::status::bad_request;
```

Семантика:

```text
http::error::body_limit
  Клиент отправил body больше допустимого. Ответ: 413 Payload Too Large.

другая ошибка parser-а
  Некорректный HTTP-запрос. Ответ: 400 Bad Request.
```

Ответ строится как JSON:

```cpp
{"error":"..."}
```

После protocol-level ошибки keep-alive отключается, потому что состояние потока уже нельзя надежно продолжать.

## Listener

`Listener` отвечает только за прием соединений:

```cpp
acceptor_.open(endpoint.protocol(), error);
acceptor_.set_option(asio::socket_base::reuse_address(true), error);
acceptor_.bind(endpoint, error);
acceptor_.listen(backlog, error);
```

Семантика:

```text
open
  Создать системный socket для acceptor.

reuse_address(true)
  Позволить быстрее переиспользовать порт после перезапуска.

bind
  Привязать socket к IP/порту.

listen
  Начать прием входящих TCP-соединений.
```

Прием соединения:

```cpp
acceptor_.async_accept(
    beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
```

После успешного accept создается session:

```cpp
std::make_shared<HttpSession>(std::move(socket), dispatcher_, settings_)->run();
```

`std::move(socket)` передает владение сокетом из listener в session.

## Пул потоков

```cpp
for (std::size_t i = 0; i < worker_count; ++i) {
  workers.emplace_back([&io] { io.run(); });
}
```

Семантика:

```text
io.run()
  Event loop Boost.Asio. Поток выполняет готовые completion handlers.

worker_count
  Сколько потоков одновременно обслуживают один общий io_context.
```

Если один worker поймает необработанное исключение, оно сохраняется, `io.stop()` останавливает event loop, потоки join-ятся, после чего исключение пробрасывается наружу. Это лучше, чем тихо потерять падение worker-потока.

## Потокобезопасность

Маршруты регистрируются до `Server::run`. После запуска `AppRouter` только читается.

`RequestDispatcher` теперь хранит:

```cpp
const AppRouter& router_;
```

Это важно семантически: dispatcher используется из нескольких worker-потоков и не должен менять таблицу маршрутов.

Каждая `HttpSession` владеет своим состоянием:

```text
beast::tcp_stream
beast::flat_buffer
RequestParser
BeastResponse
```

Клиенты не делят parser или response buffer между собой.

## RequestDispatcher как HTTP boundary

Dispatcher переводит Beast request в проектный `HttpRequest`, но теперь маршрутизирует только path:

```cpp
const auto query_position = target.find('?');
if (query_position != std::string::npos) {
  target.resize(query_position);
}
```

Семантика:

```text
/health?verbose=true
  HTTP target, пришедший от клиента.

/health
  path, который ищется в AppRouter.
```

Это поведение ближе к production web frameworks: query string не должен ломать совпадение маршрута.

Dispatcher также добавляет:

```cpp
response.set("X-Content-Type-Options", "nosniff");
```

Смысл: браузеру запрещается угадывать MIME-тип ответа вместо доверия `Content-Type`.

## Безопасный JSON ошибок

Раньше ошибка собиралась вручную:

```cpp
std::string{R"({"error":")"} + exception.what() + R"("})"
```

Такой код ломает JSON, если текст исключения содержит кавычки. Теперь используется `nlohmann::json`:

```cpp
nlohmann::json body;
body["error"] = std::move(message);
return body.dump();
```

Семантика: библиотека сама экранирует спецсимволы, а клиент всегда получает корректный JSON.

## Итоговая архитектура

```text
main()
  -> settings_from_env_and_cli()
  -> Server::run(settings)
  -> io_context
  -> Listener::async_accept
  -> HttpSession per connection
  -> request_parser with body_limit
  -> http::async_read
  -> RequestDispatcher
  -> path normalization
  -> AppRouter
  -> handler/controller/database
  -> http::async_write
  -> keep-alive read again или shutdown
```

## Проверка

Выполнено:

```powershell
cmake --build server\build
ctest --test-dir server\build --output-on-failure
```

Результат:

```text
100% tests passed, 0 tests failed out of 16
```
