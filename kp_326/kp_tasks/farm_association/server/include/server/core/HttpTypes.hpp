#pragma once

#include <string>
#include <unordered_map>

namespace fasc::server::core {

///Проектное представление HTTP-запроса после преобразования из транспортного слоя.
///@note Не зависит от Boost.Beast и используется всеми слоями выше @c RequestDispatcher.
struct HttpRequest {
  ///HTTP-метод запроса.
  std::string method;

  ///Путь маршрута без query string.
  std::string target;

  ///Тело запроса.
  std::string body;

  ///HTTP-заголовки запроса.
  std::unordered_map<std::string, std::string> headers;

  ///Параметры query string после URL-decoding.
  std::unordered_map<std::string, std::string> query_params;
};

///Проектное представление HTTP-ответа перед преобразованием в Boost.Beast response.
///@note Содержит только данные, которые нужны application HTTP boundary.
struct HttpResponse {
  ///HTTP-статус ответа.
  unsigned status{200};

  ///Значение заголовка @c Content-Type.
  std::string content_type{"application/json"};

  ///Тело ответа.
  std::string body;
};

} // namespace fasc::server::core
