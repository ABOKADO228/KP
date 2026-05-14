#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmPlotAssignment.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmPlotAssignmentRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotAssignmentRowsDto, FarmEntityError>;
using FarmPlotAssignmentRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotAssignmentRowDto, FarmEntityError>;
using FarmPlotAssignmentMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmPlotAssignmentMutationDto, FarmEntityError>;

/// Application controller таблицы farm_plot_assignment.
class FarmPlotAssignmentController {
public:
  /// Создает controller таблицы farm_plot_assignment.
  ///@param db база данных приложения.
  explicit FarmPlotAssignmentController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmPlotAssignmentRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmPlotAssignmentRowResult load(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmPlotAssignmentMutationResult create(const fasc::server::controllers::dto::FarmPlotAssignmentCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmPlotAssignmentMutationResult update(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key,
                             const fasc::server::controllers::dto::FarmPlotAssignmentUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmPlotAssignmentMutationResult erase(const fasc::server::controllers::dto::FarmPlotAssignmentKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
