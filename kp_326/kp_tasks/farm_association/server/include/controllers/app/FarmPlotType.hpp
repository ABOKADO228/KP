#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmPlotType.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmPlotTypeRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotTypeRowsDto, FarmEntityError>;
using FarmPlotTypeRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotTypeRowDto, FarmEntityError>;
using FarmPlotTypeMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotTypeMutationDto, FarmEntityError>;

/// Application controller таблицы farm_plot_type.
class FarmPlotTypeController {
public:
  /// Создает controller таблицы farm_plot_type.
  ///@param db база данных приложения.
  explicit FarmPlotTypeController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmPlotTypeRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmPlotTypeRowResult load(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmPlotTypeMutationResult create(const fasc::server::controllers::dto::FarmPlotTypeCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmPlotTypeMutationResult update(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key,
                             const fasc::server::controllers::dto::FarmPlotTypeUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmPlotTypeMutationResult erase(const fasc::server::controllers::dto::FarmPlotTypeKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
