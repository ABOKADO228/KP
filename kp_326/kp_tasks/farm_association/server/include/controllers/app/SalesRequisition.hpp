#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/SalesRequisition.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using SalesRequisitionRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SalesRequisitionRowsDto, FarmEntityError>;
using SalesRequisitionRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SalesRequisitionRowDto, FarmEntityError>;
using SalesRequisitionMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::SalesRequisitionMutationDto, FarmEntityError>;

/// Application controller таблицы sales_requisition.
class SalesRequisitionController {
public:
  /// Создает controller таблицы sales_requisition.
  ///@param db база данных приложения.
  explicit SalesRequisitionController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  SalesRequisitionRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  SalesRequisitionRowResult load(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  SalesRequisitionMutationResult create(const fasc::server::controllers::dto::SalesRequisitionCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  SalesRequisitionMutationResult update(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key,
                             const fasc::server::controllers::dto::SalesRequisitionUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  SalesRequisitionMutationResult erase(const fasc::server::controllers::dto::SalesRequisitionKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
