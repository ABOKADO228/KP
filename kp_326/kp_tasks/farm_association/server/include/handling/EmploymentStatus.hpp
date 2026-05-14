#pragma once

#include <database/Database.hpp>
#include <server/core/Server.hpp>

#include <controllers/http/EmploymentStatus.hpp>
#include <server/core/HttpTypes.hpp>

namespace fasc::server::handling {

/// Handler таблицы employment_status.
class EmploymentStatusHandler {
public:
  /// Создает handler таблицы.
  ///@param controller HTTP controller таблицы.
  explicit EmploymentStatusHandler(fasc::server::controllers::http::EmploymentStatusHttpController& controller);

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
  fasc::server::controllers::http::EmploymentStatusHttpController& controller_;
};

/// Registers HTTP routes for the EmploymentStatus table.
///@param server HTTP server.
///@param database application database.
void registerEmploymentStatusRoutes(fasc::server::core::Server& server,
                           fasc::server::database::Database& database);

} // namespace fasc::server::handling
