#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmRole.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmRoleRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmRoleRowsDto, FarmEntityError>;
using FarmRoleRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmRoleRowDto, FarmEntityError>;
using FarmRoleMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmRoleMutationDto, FarmEntityError>;

/// Application controller таблицы farm_role.
class FarmRoleController {
public:
  /// Создает controller таблицы farm_role.
  ///@param db база данных приложения.
  explicit FarmRoleController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmRoleRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmRoleRowResult load(const fasc::server::controllers::dto::FarmRoleKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmRoleMutationResult create(const fasc::server::controllers::dto::FarmRoleCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmRoleMutationResult update(const fasc::server::controllers::dto::FarmRoleKeyDto& key,
                             const fasc::server::controllers::dto::FarmRoleUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmRoleMutationResult erase(const fasc::server::controllers::dto::FarmRoleKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
