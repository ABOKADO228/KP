#pragma once

#include <controllers/app/FarmPlotProductionProduct.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmPlotProductionProduct.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmPlotProductionProductRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotProductionProductRowsView, fasc::server::views::ErrorView>;
using FarmPlotProductionProductRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotProductionProductRowView, fasc::server::views::ErrorView>;
using FarmPlotProductionProductMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotProductionProductMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_plot_production_product.
class FarmPlotProductionProductHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmPlotProductionProductHttpController(fasc::server::controllers::app::FarmPlotProductionProductController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmPlotProductionProductRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmPlotProductionProductRowViewResult load(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotProductionProductMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotProductionProductMutationViewResult update(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmPlotProductionProductMutationViewResult erase(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmPlotProductionProductController& controller_;
};

} // namespace fasc::server::controllers::http
