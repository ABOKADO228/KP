#pragma once

#include <string>

namespace fasc::server::views {

///Код ошибки на границе HTTP controller -> handling.
enum class ErrorViewCode {
  ///Некорректный запрос клиента.
  BadRequest,

  ///Ошибка авторизации.
  Unauthorized,

  ///Запрошенная сущность не найдена.
  NotFound,

  ///Конфликт состояния или уникальности.
  Conflict,

  ///Непредвиденная ошибка сервера.
  InternalServerError,
};

///Публичное представление ошибки API.
///@note В JSON сериализуется только сообщение, а код используется handler-ом для HTTP-статуса.
struct ErrorView {
  ///Код ошибки для выбора HTTP-статуса.
  ErrorViewCode code{ErrorViewCode::BadRequest};

  ///Сообщение ошибки для тела ответа.
  std::string message;
};

} // namespace fasc::server::views
