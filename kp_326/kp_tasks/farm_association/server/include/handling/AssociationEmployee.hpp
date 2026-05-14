#pragma once

#include <database/Database.hpp>
#include <server/core/Server.hpp>

#include <controllers/http/AssociationEmployee.hpp>
#include <server/core/HttpTypes.hpp>

namespace fasc::server::handling {

/// Handler таблицы association_employee.
class AssociationEmployeeHandler {
public:
  /// Создает handler таблицы.
  ///@param controller HTTP controller таблицы.
  explicit AssociationEmployeeHandler(fasc::server::controllers::http::AssociationEmployeeHttpController& controller);

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
  fasc::server::controllers::http::AssociationEmployeeHttpController& controller_;
};

/// Registers HTTP routes for the AssociationEmployee table.
///@param server HTTP server.
///@param database application database.
void registerAssociationEmployeeRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database);

} // namespace fasc::server::handling
