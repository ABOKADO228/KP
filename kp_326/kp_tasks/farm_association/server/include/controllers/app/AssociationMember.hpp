#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/AssociationMember.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using AssociationMemberRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationMemberRowsDto, FarmEntityError>;
using AssociationMemberRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationMemberRowDto, FarmEntityError>;
using AssociationMemberMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationMemberMutationDto, FarmEntityError>;

/// Application controller таблицы association_member.
class AssociationMemberController {
public:
  /// Создает controller таблицы association_member.
  ///@param db база данных приложения.
  explicit AssociationMemberController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  AssociationMemberRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  AssociationMemberRowResult load(const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  AssociationMemberMutationResult create(const fasc::server::controllers::dto::AssociationMemberCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  AssociationMemberMutationResult update(const fasc::server::controllers::dto::AssociationMemberKeyDto& key,
                             const fasc::server::controllers::dto::AssociationMemberUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  AssociationMemberMutationResult erase(const fasc::server::controllers::dto::AssociationMemberKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
