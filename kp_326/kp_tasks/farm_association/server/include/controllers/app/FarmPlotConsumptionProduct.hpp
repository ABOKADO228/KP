#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmPlotConsumptionProduct.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmPlotConsumptionProductRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotConsumptionProductRowsDto, FarmEntityError>;
using FarmPlotConsumptionProductRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotConsumptionProductRowDto, FarmEntityError>;
using FarmPlotConsumptionProductMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotConsumptionProductMutationDto, FarmEntityError>;

/// Application controller таблицы farm_plot_consumption_product.
class FarmPlotConsumptionProductController {
public:
  /// Создает controller таблицы farm_plot_consumption_product.
  ///@param db база данных приложения.
  explicit FarmPlotConsumptionProductController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmPlotConsumptionProductRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmPlotConsumptionProductRowResult load(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmPlotConsumptionProductMutationResult create(const fasc::server::controllers::dto::FarmPlotConsumptionProductCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmPlotConsumptionProductMutationResult update(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key,
                             const fasc::server::controllers::dto::FarmPlotConsumptionProductUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmPlotConsumptionProductMutationResult erase(const fasc::server::controllers::dto::FarmPlotConsumptionProductKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
