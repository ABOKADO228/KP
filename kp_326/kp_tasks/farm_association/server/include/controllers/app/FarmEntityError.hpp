#pragma once

#include <string>

namespace fasc::server::controllers::app {

/// Код ошибки операции над таблицей фермерской ассоциации.
enum class FarmEntityErrorCode {
  /// В запросе отсутствуют обязательные данные.
  InvalidInput,

  /// Запрошенная строка не найдена.
  NotFound,

  /// База данных вернула ошибку.
  PersistenceFailure,
};

/// Ошибка application controller-а таблицы.
struct FarmEntityError {
  /// Код ошибки для HTTP-маппинга.
  FarmEntityErrorCode code{FarmEntityErrorCode::InvalidInput};

  /// Текст ошибки.
  std::string message;
};

} // namespace fasc::server::controllers::app
