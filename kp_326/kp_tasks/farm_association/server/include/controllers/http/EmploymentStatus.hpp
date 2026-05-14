#pragma once

#include <controllers/app/EmploymentStatus.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/EmploymentStatus.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using EmploymentStatusRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::EmploymentStatusRowsView, fasc::server::views::ErrorView>;
using EmploymentStatusRowViewResult = fasc::extended::fpp::Result<fasc::server::views::EmploymentStatusRowView, fasc::server::views::ErrorView>;
using EmploymentStatusMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::EmploymentStatusMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы employment_status.
class EmploymentStatusHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit EmploymentStatusHttpController(fasc::server::controllers::app::EmploymentStatusController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  EmploymentStatusRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  EmploymentStatusRowViewResult load(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  EmploymentStatusMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  EmploymentStatusMutationViewResult update(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  EmploymentStatusMutationViewResult erase(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const;

private:
  fasc::server::controllers::app::EmploymentStatusController& controller_;
};

} // namespace fasc::server::controllers::http
