#pragma once

#include <string>
#include <unordered_map>

///HTTP-запрос, переданный из транспортного слоя в application routing.
///@note Структура не владеет сетевым соединением и хранит только данные, нужные обработчикам.
struct HttpRequest {
  ///HTTP-метод запроса, например @c GET или @c POST.
  std::string method;

  ///Целевой путь запроса, например @c /health или @c /users.
  std::string target;

  ///Тело запроса в текстовом виде.
  std::string body;

  ///HTTP-заголовки запроса.
  std::unordered_map<std::string, std::string> headers;
};

///HTTP-ответ, который application layer возвращает транспортному слою.
///@note Поле @c body должно уже содержать готовое представление ответа.
struct HttpResponse {
  ///HTTP-статус ответа.
  unsigned status{200};

  ///MIME-тип тела ответа.
  std::string content_type{"application/json"};

  ///Тело ответа.
  std::string body;
};
