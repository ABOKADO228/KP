#pragma once

#include <controllers/app/FarmPlotConsumptionProduct.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmPlotConsumptionProduct.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmPlotConsumptionProductRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotConsumptionProductRowsView, fasc::server::views::ErrorView>;
using FarmPlotConsumptionProductRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotConsumptionProductRowView, fasc::server::views::ErrorView>;
using FarmPlotConsumptionProductMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmPlotConsumptionProductMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_plot_consumption_product.
class FarmPlotConsumptionProductHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmPlotConsumptionProductHttpController(fasc::server::controllers::app::FarmPlotConsumptionProductController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmPlotConsumptionProductRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmPlotConsumptionProductRowViewResult load(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotConsumptionProductMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmPlotConsumptionProductMutationViewResult update(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmPlotConsumptionProductMutationViewResult erase(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmPlotConsumptionProductController& controller_;
};

} // namespace fasc::server::controllers::http
