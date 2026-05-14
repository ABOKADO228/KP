#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/Person.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using PersonRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PersonRowsDto, FarmEntityError>;
using PersonRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PersonRowDto, FarmEntityError>;
using PersonMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PersonMutationDto, FarmEntityError>;

/// Application controller таблицы person.
class PersonController {
public:
  /// Создает controller таблицы person.
  ///@param db база данных приложения.
  explicit PersonController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  PersonRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  PersonRowResult load(const fasc::server::controllers::dto::PersonKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  PersonMutationResult create(const fasc::server::controllers::dto::PersonCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  PersonMutationResult update(const fasc::server::controllers::dto::PersonKeyDto& key,
                             const fasc::server::controllers::dto::PersonUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  PersonMutationResult erase(const fasc::server::controllers::dto::PersonKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
