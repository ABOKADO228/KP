#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmAssociation.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmAssociationRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmAssociationRowsDto, FarmEntityError>;
using FarmAssociationRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmAssociationRowDto, FarmEntityError>;
using FarmAssociationMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmAssociationMutationDto, FarmEntityError>;

/// Application controller таблицы farm_association.
class FarmAssociationController {
public:
  /// Создает controller таблицы farm_association.
  ///@param db база данных приложения.
  explicit FarmAssociationController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmAssociationRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmAssociationRowResult load(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmAssociationMutationResult create(const fasc::server::controllers::dto::FarmAssociationCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmAssociationMutationResult update(const fasc::server::controllers::dto::FarmAssociationKeyDto& key,
                             const fasc::server::controllers::dto::FarmAssociationUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmAssociationMutationResult erase(const fasc::server::controllers::dto::FarmAssociationKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
