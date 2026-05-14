#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmOwner.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmOwnerRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmOwnerRowsDto, FarmEntityError>;
using FarmOwnerRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmOwnerRowDto, FarmEntityError>;
using FarmOwnerMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmOwnerMutationDto, FarmEntityError>;

/// Application controller таблицы farm_owner.
class FarmOwnerController {
public:
  /// Создает controller таблицы farm_owner.
  ///@param db база данных приложения.
  explicit FarmOwnerController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmOwnerRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmOwnerRowResult load(const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmOwnerMutationResult create(const fasc::server::controllers::dto::FarmOwnerCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmOwnerMutationResult update(const fasc::server::controllers::dto::FarmOwnerKeyDto& key,
                             const fasc::server::controllers::dto::FarmOwnerUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmOwnerMutationResult erase(const fasc::server::controllers::dto::FarmOwnerKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
