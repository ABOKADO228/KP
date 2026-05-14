#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/Supplier.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using SupplierRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SupplierRowsDto, FarmEntityError>;
using SupplierRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SupplierRowDto, FarmEntityError>;
using SupplierMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SupplierMutationDto, FarmEntityError>;

/// Application controller таблицы supplier.
class SupplierController {
public:
  /// Создает controller таблицы supplier.
  ///@param db база данных приложения.
  explicit SupplierController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  SupplierRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  SupplierRowResult load(const fasc::server::controllers::dto::SupplierKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  SupplierMutationResult create(const fasc::server::controllers::dto::SupplierCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  SupplierMutationResult update(const fasc::server::controllers::dto::SupplierKeyDto& key,
                             const fasc::server::controllers::dto::SupplierUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  SupplierMutationResult erase(const fasc::server::controllers::dto::SupplierKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
