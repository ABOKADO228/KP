#pragma once

#include <server/core/AppRouter.hpp>

#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

using BeastRequest = http::request<http::string_body>;
using BeastResponse = http::response<http::string_body>;

///Преобразует запросы Boost.Beast в проектные @c HttpRequest и обратно в @c BeastResponse.
///@note Класс связывает транспортный слой с @c AppRouter.
class RequestDispatcher {
public:
  ///Создает dispatcher поверх существующего маршрутизатора.
  ///@param router маршрутизатор, который будет обрабатывать проектные HTTP-запросы.
  explicit RequestDispatcher(const AppRouter& router);

  ///Обрабатывает входящий Boost.Beast-запрос.
  ///@param request исходный HTTP-запрос Boost.Beast.
  ///@returns HTTP-ответ Boost.Beast, готовый для отправки клиенту.
  BeastResponse dispatch(const BeastRequest& request) const;

private:
  const AppRouter& router_;
};
