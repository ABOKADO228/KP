#pragma once

#include <controllers/app/FarmPlotType.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmPlotType.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmPlotTypeRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotTypeRowsView, fasc::server::views::ErrorView>;
using FarmPlotTypeRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotTypeRowView, fasc::server::views::ErrorView>;
using FarmPlotTypeMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotTypeMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_plot_type.
class FarmPlotTypeHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmPlotTypeHttpController(fasc::server::controllers::app::FarmPlotTypeController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmPlotTypeRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmPlotTypeRowViewResult load(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotTypeMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotTypeMutationViewResult update(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmPlotTypeMutationViewResult erase(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmPlotTypeController& controller_;
};

} // namespace fasc::server::controllers::http
