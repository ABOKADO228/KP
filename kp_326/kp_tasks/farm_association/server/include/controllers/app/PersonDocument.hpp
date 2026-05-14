#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/PersonDocument.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using PersonDocumentRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PersonDocumentRowsDto, FarmEntityError>;
using PersonDocumentRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PersonDocumentRowDto, FarmEntityError>;
using PersonDocumentMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PersonDocumentMutationDto, FarmEntityError>;

/// Application controller таблицы person_document.
class PersonDocumentController {
public:
  /// Создает controller таблицы person_document.
  ///@param db база данных приложения.
  explicit PersonDocumentController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  PersonDocumentRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  PersonDocumentRowResult load(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  PersonDocumentMutationResult create(const fasc::server::controllers::dto::PersonDocumentCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  PersonDocumentMutationResult update(const fasc::server::controllers::dto::PersonDocumentKeyDto& key,
                             const fasc::server::controllers::dto::PersonDocumentUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  PersonDocumentMutationResult erase(const fasc::server::controllers::dto::PersonDocumentKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
