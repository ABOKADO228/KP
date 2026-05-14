#pragma once

#include <controllers/app/FarmPlotAssignment.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmPlotAssignment.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmPlotAssignmentRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotAssignmentRowsView, fasc::server::views::ErrorView>;
using FarmPlotAssignmentRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotAssignmentRowView, fasc::server::views::ErrorView>;
using FarmPlotAssignmentMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotAssignmentMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_plot_assignment.
class FarmPlotAssignmentHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmPlotAssignmentHttpController(fasc::server::controllers::app::FarmPlotAssignmentController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmPlotAssignmentRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmPlotAssignmentRowViewResult load(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotAssignmentMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotAssignmentMutationViewResult update(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmPlotAssignmentMutationViewResult erase(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmPlotAssignmentController& controller_;
};

} // namespace fasc::server::controllers::http
