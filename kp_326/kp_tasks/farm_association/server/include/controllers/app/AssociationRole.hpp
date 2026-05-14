#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/AssociationRole.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using AssociationRoleRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationRoleRowsDto, FarmEntityError>;
using AssociationRoleRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationRoleRowDto, FarmEntityError>;
using AssociationRoleMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationRoleMutationDto, FarmEntityError>;

/// Application controller таблицы association_role.
class AssociationRoleController {
public:
  /// Создает controller таблицы association_role.
  ///@param db база данных приложения.
  explicit AssociationRoleController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  AssociationRoleRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  AssociationRoleRowResult load(const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  AssociationRoleMutationResult create(const fasc::server::controllers::dto::AssociationRoleCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  AssociationRoleMutationResult update(const fasc::server::controllers::dto::AssociationRoleKeyDto& key,
                             const fasc::server::controllers::dto::AssociationRoleUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  AssociationRoleMutationResult erase(const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
