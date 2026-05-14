#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmOwnership.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmOwnershipRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmOwnershipRowsDto, FarmEntityError>;
using FarmOwnershipRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmOwnershipRowDto, FarmEntityError>;
using FarmOwnershipMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmOwnershipMutationDto, FarmEntityError>;

/// Application controller таблицы farm_ownership.
class FarmOwnershipController {
public:
  /// Создает controller таблицы farm_ownership.
  ///@param db база данных приложения.
  explicit FarmOwnershipController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmOwnershipRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmOwnershipRowResult load(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmOwnershipMutationResult create(const fasc::server::controllers::dto::FarmOwnershipCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmOwnershipMutationResult update(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key,
                             const fasc::server::controllers::dto::FarmOwnershipUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmOwnershipMutationResult erase(const fasc::server::controllers::dto::FarmOwnershipKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
