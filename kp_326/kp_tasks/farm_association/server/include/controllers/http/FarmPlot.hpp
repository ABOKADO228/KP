#pragma once

#include <controllers/app/FarmPlot.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmPlot.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmPlotRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotRowsView, fasc::server::views::ErrorView>;
using FarmPlotRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotRowView, fasc::server::views::ErrorView>;
using FarmPlotMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_plot.
class FarmPlotHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmPlotHttpController(fasc::server::controllers::app::FarmPlotController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmPlotRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmPlotRowViewResult load(const fasc::server::controllers::dto::FarmPlotKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotMutationViewResult update(const fasc::server::controllers::dto::FarmPlotKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmPlotMutationViewResult erase(const fasc::server::controllers::dto::FarmPlotKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmPlotController& controller_;
};

} // namespace fasc::server::controllers::http
