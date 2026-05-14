#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmPlot.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmPlotRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotRowsDto, FarmEntityError>;
using FarmPlotRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotRowDto, FarmEntityError>;
using FarmPlotMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotMutationDto, FarmEntityError>;

/// Application controller таблицы farm_plot.
class FarmPlotController {
public:
  /// Создает controller таблицы farm_plot.
  ///@param db база данных приложения.
  explicit FarmPlotController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmPlotRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmPlotRowResult load(const fasc::server::controllers::dto::FarmPlotKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmPlotMutationResult create(const fasc::server::controllers::dto::FarmPlotCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmPlotMutationResult update(const fasc::server::controllers::dto::FarmPlotKeyDto& key,
                             const fasc::server::controllers::dto::FarmPlotUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmPlotMutationResult erase(const fasc::server::controllers::dto::FarmPlotKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
