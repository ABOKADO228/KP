#pragma once

#include <server/core/HttpTypes.hpp>

/// Handler health-check маршрута.
///@note Не обращается к базе данных и используется для простой проверки доступности сервера.
class HealthHandler {
public:
  /// Возвращает health-check ответ.
  ///@param request HTTP-запрос health-check маршрута.
  ///@returns HTTP-ответ со статусом здоровья сервера.
  HttpResponse health(const HttpRequest& request);
};
