#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/Farm.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmRowsDto, FarmEntityError>;
using FarmRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmRowDto, FarmEntityError>;
using FarmMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmMutationDto, FarmEntityError>;

/// Application controller таблицы farm.
class FarmController {
public:
  /// Создает controller таблицы farm.
  ///@param db база данных приложения.
  explicit FarmController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmRowResult load(const fasc::server::controllers::dto::FarmKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmMutationResult create(const fasc::server::controllers::dto::FarmCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmMutationResult update(const fasc::server::controllers::dto::FarmKeyDto& key,
                             const fasc::server::controllers::dto::FarmUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmMutationResult erase(const fasc::server::controllers::dto::FarmKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
