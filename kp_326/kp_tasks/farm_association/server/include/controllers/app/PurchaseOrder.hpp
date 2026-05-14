#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/PurchaseOrder.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using PurchaseOrderRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseOrderRowsDto, FarmEntityError>;
using PurchaseOrderRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseOrderRowDto, FarmEntityError>;
using PurchaseOrderMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseOrderMutationDto, FarmEntityError>;

/// Application controller таблицы purchase_order.
class PurchaseOrderController {
public:
  /// Создает controller таблицы purchase_order.
  ///@param db база данных приложения.
  explicit PurchaseOrderController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  PurchaseOrderRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  PurchaseOrderRowResult load(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  PurchaseOrderMutationResult create(const fasc::server::controllers::dto::PurchaseOrderCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  PurchaseOrderMutationResult update(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key,
                             const fasc::server::controllers::dto::PurchaseOrderUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  PurchaseOrderMutationResult erase(const fasc::server::controllers::dto::PurchaseOrderKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
