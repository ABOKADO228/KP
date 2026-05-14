#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/IdentityDocumentType.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using IdentityDocumentTypeRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::IdentityDocumentTypeRowsDto, FarmEntityError>;
using IdentityDocumentTypeRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::IdentityDocumentTypeRowDto, FarmEntityError>;
using IdentityDocumentTypeMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::IdentityDocumentTypeMutationDto, FarmEntityError>;

/// Application controller таблицы identity_document_type.
class IdentityDocumentTypeController {
public:
  /// Создает controller таблицы identity_document_type.
  ///@param db база данных приложения.
  explicit IdentityDocumentTypeController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  IdentityDocumentTypeRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  IdentityDocumentTypeRowResult load(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  IdentityDocumentTypeMutationResult create(const fasc::server::controllers::dto::IdentityDocumentTypeCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  IdentityDocumentTypeMutationResult update(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key,
                             const fasc::server::controllers::dto::IdentityDocumentTypeUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  IdentityDocumentTypeMutationResult erase(const fasc::server::controllers::dto::IdentityDocumentTypeKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
