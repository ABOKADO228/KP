#pragma once

#include <database/Database.hpp>
#include <server/core/Server.hpp>

#include <controllers/http/PurchaseRequisition.hpp>
#include <server/core/HttpTypes.hpp>

namespace fasc::server::handling {

/// Handler таблицы purchase_requisition.
class PurchaseRequisitionHandler {
public:
  /// Создает handler таблицы.
  ///@param controller HTTP controller таблицы.
  explicit PurchaseRequisitionHandler(fasc::server::controllers::http::PurchaseRequisitionHttpController& controller);

  /// Формирует ответ списка.
  ///@param request HTTP-запрос.
  ///@returns HTTP-ответ.
  fasc::server::core::HttpResponse list(const fasc::server::core::HttpRequest& request);

  /// Формирует ответ строки.
  ///@param request HTTP-запрос.
  ///@returns HTTP-ответ.
  fasc::server::core::HttpResponse load(const fasc::server::core::HttpRequest& request);

  /// Формирует ответ создания.
  ///@param request HTTP-запрос.
  ///@returns HTTP-ответ.
  fasc::server::core::HttpResponse create(const fasc::server::core::HttpRequest& request);

  /// Формирует ответ обновления.
  ///@param request HTTP-запрос.
  ///@returns HTTP-ответ.
  fasc::server::core::HttpResponse update(const fasc::server::core::HttpRequest& request);

  /// Формирует ответ удаления.
  ///@param request HTTP-запрос.
  ///@returns HTTP-ответ.
  fasc::server::core::HttpResponse erase(const fasc::server::core::HttpRequest& request);

private:
  fasc::server::controllers::http::PurchaseRequisitionHttpController& controller_;
};

/// Registers HTTP routes for the PurchaseRequisition table.
///@param server HTTP server.
///@param database application database.
void registerPurchaseRequisitionRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database);

} // namespace fasc::server::handling
