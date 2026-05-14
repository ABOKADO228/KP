#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/Contract.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using ContractRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ContractRowsDto, FarmEntityError>;
using ContractRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ContractRowDto, FarmEntityError>;
using ContractMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::ContractMutationDto, FarmEntityError>;

/// Application controller таблицы contract.
class ContractController {
public:
  /// Создает controller таблицы contract.
  ///@param db база данных приложения.
  explicit ContractController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  ContractRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  ContractRowResult load(const fasc::server::controllers::dto::ContractKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  ContractMutationResult create(const fasc::server::controllers::dto::ContractCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  ContractMutationResult update(const fasc::server::controllers::dto::ContractKeyDto& key,
                             const fasc::server::controllers::dto::ContractUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  ContractMutationResult erase(const fasc::server::controllers::dto::ContractKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
