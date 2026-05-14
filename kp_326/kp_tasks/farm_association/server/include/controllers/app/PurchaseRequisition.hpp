#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/PurchaseRequisition.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using PurchaseRequisitionRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseRequisitionRowsDto, FarmEntityError>;
using PurchaseRequisitionRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseRequisitionRowDto, FarmEntityError>;
using PurchaseRequisitionMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::PurchaseRequisitionMutationDto, FarmEntityError>;

/// Application controller таблицы purchase_requisition.
class PurchaseRequisitionController {
public:
  /// Создает controller таблицы purchase_requisition.
  ///@param db база данных приложения.
  explicit PurchaseRequisitionController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  PurchaseRequisitionRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  PurchaseRequisitionRowResult load(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  PurchaseRequisitionMutationResult create(const fasc::server::controllers::dto::PurchaseRequisitionCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  PurchaseRequisitionMutationResult update(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key,
                             const fasc::server::controllers::dto::PurchaseRequisitionUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  PurchaseRequisitionMutationResult erase(const fasc::server::controllers::dto::PurchaseRequisitionKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
