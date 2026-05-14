#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/AssociationFarms.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using AssociationFarmsRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationFarmsRowsDto, FarmEntityError>;
using AssociationFarmsRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationFarmsRowDto, FarmEntityError>;
using AssociationFarmsMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationFarmsMutationDto, FarmEntityError>;

/// Application controller таблицы association_farms.
class AssociationFarmsController {
public:
  /// Создает controller таблицы association_farms.
  ///@param db база данных приложения.
  explicit AssociationFarmsController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  AssociationFarmsRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  AssociationFarmsRowResult load(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  AssociationFarmsMutationResult create(const fasc::server::controllers::dto::AssociationFarmsCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  AssociationFarmsMutationResult update(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key,
                             const fasc::server::controllers::dto::AssociationFarmsUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  AssociationFarmsMutationResult erase(const fasc::server::controllers::dto::AssociationFarmsKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
