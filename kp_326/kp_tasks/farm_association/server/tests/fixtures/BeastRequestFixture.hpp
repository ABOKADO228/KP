#pragma once

#include <server/core/RequestDispatcher.hpp>

#include <boost/beast/http.hpp>

#include <string>

namespace fasc::server::tests::fixtures {

using fasc::server::core::BeastRequest;

///Создает Boost.Beast HTTP-запрос для integration-тестов dispatcher-а.
///@param method HTTP-метод Boost.Beast.
///@param target путь запроса.
///@param body   тело запроса.
///@returns заполненный @c BeastRequest.
inline BeastRequest makeBeastRequest(boost::beast::http::verb method,
                                      std::string target,
                                      std::string body = {}) {
  BeastRequest request{method, std::move(target), 11};
  request.set(boost::beast::http::field::host, "localhost");
  request.set(boost::beast::http::field::content_type, "application/json");
  request.body() = std::move(body);
  request.prepare_payload();
  return request;
}

} // namespace fasc::server::tests::fixtures
