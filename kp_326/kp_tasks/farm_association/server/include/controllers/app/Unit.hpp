#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/Unit.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using UnitRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::UnitRowsDto, FarmEntityError>;
using UnitRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::UnitRowDto, FarmEntityError>;
using UnitMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::UnitMutationDto, FarmEntityError>;

/// Application controller таблицы unit.
class UnitController {
public:
  /// Создает controller таблицы unit.
  ///@param db база данных приложения.
  explicit UnitController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  UnitRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  UnitRowResult load(const fasc::server::controllers::dto::UnitKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  UnitMutationResult create(const fasc::server::controllers::dto::UnitCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  UnitMutationResult update(const fasc::server::controllers::dto::UnitKeyDto& key,
                             const fasc::server::controllers::dto::UnitUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  UnitMutationResult erase(const fasc::server::controllers::dto::UnitKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
