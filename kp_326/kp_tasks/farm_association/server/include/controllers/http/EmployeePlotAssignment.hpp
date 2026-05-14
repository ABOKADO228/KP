#pragma once

#include <controllers/app/EmployeePlotAssignment.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/EmployeePlotAssignment.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using EmployeePlotAssignmentRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::EmployeePlotAssignmentRowsView, fasc::server::views::ErrorView>;
using EmployeePlotAssignmentRowViewResult = fasc::extended::fpp::Result<fasc::server::views::EmployeePlotAssignmentRowView, fasc::server::views::ErrorView>;
using EmployeePlotAssignmentMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::EmployeePlotAssignmentMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы employee_plot_assignment.
class EmployeePlotAssignmentHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit EmployeePlotAssignmentHttpController(fasc::server::controllers::app::EmployeePlotAssignmentController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  EmployeePlotAssignmentRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  EmployeePlotAssignmentRowViewResult load(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  EmployeePlotAssignmentMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  EmployeePlotAssignmentMutationViewResult update(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  EmployeePlotAssignmentMutationViewResult erase(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const;

private:
  fasc::server::controllers::app::EmployeePlotAssignmentController& controller_;
};

} // namespace fasc::server::controllers::http
