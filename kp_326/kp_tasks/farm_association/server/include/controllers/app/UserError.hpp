#pragma once

#include <string>

namespace fasc::server::controllers::app {

///Код предметной ошибки пользовательского сценария.
enum class UserErrorCode {
  ///Входные данные не прошли валидацию.
  InvalidInput,

  ///Запрашиваемый пользователь или учетные данные не найдены.
  Unauthorized,

  ///Операция конфликтует с текущим состоянием данных.
  Conflict,

  ///Persistence layer вернул ошибку выполнения операции.
  PersistenceFailure,
};

///Предметная ошибка пользовательского application controller-а.
///@note Не содержит HTTP-статус и JSON-представление.
struct UserError {
  ///Код ошибки для последующего маппинга в HTTP layer.
  UserErrorCode code{UserErrorCode::InvalidInput};

  ///Сообщение ошибки для внешнего error view.
  std::string message;
};

} // namespace fasc::server::controllers::app
