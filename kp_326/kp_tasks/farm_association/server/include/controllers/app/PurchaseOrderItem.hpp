#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/PurchaseOrderItem.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using PurchaseOrderItemRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseOrderItemRowsDto, FarmEntityError>;
using PurchaseOrderItemRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseOrderItemRowDto, FarmEntityError>;
using PurchaseOrderItemMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseOrderItemMutationDto, FarmEntityError>;

/// Application controller таблицы purchase_order_item.
class PurchaseOrderItemController {
public:
  /// Создает controller таблицы purchase_order_item.
  ///@param db база данных приложения.
  explicit PurchaseOrderItemController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  PurchaseOrderItemRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  PurchaseOrderItemRowResult load(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  PurchaseOrderItemMutationResult create(const fasc::server::controllers::dto::PurchaseOrderItemCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  PurchaseOrderItemMutationResult update(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key,
                             const fasc::server::controllers::dto::PurchaseOrderItemUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  PurchaseOrderItemMutationResult erase(const fasc::server::controllers::dto::PurchaseOrderItemKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
