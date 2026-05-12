#pragma once

#include <server/core/HttpTypes.hpp>

#include <string>

namespace fasc::server::tests::fixtures {

///Создает проектный HTTP-запрос для тестов.
///@param method HTTP-метод запроса.
///@param target путь запроса.
///@param body   тело запроса.
///@returns заполненный @c HttpRequest.
inline HttpRequest makeHttpRequest(std::string method, std::string target, std::string body = {}) {
  HttpRequest request;
  request.method = std::move(method);
  request.target = std::move(target);
  request.body = std::move(body);
  return request;
}

} // namespace fasc::server::tests::fixtures
