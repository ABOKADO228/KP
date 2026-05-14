#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmPlotProductionProduct.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmPlotProductionProductRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotProductionProductRowsDto, FarmEntityError>;
using FarmPlotProductionProductRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotProductionProductRowDto, FarmEntityError>;
using FarmPlotProductionProductMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotProductionProductMutationDto, FarmEntityError>;

/// Application controller таблицы farm_plot_production_product.
class FarmPlotProductionProductController {
public:
  /// Создает controller таблицы farm_plot_production_product.
  ///@param db база данных приложения.
  explicit FarmPlotProductionProductController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmPlotProductionProductRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmPlotProductionProductRowResult load(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmPlotProductionProductMutationResult create(const fasc::server::controllers::dto::FarmPlotProductionProductCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmPlotProductionProductMutationResult update(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key,
                             const fasc::server::controllers::dto::FarmPlotProductionProductUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmPlotProductionProductMutationResult erase(const fasc::server::controllers::dto::FarmPlotProductionProductKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
