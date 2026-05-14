#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/EmployeePlotAssignment.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using EmployeePlotAssignmentRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::EmployeePlotAssignmentRowsDto, FarmEntityError>;
using EmployeePlotAssignmentRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::EmployeePlotAssignmentRowDto, FarmEntityError>;
using EmployeePlotAssignmentMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::EmployeePlotAssignmentMutationDto, FarmEntityError>;

/// Application controller таблицы employee_plot_assignment.
class EmployeePlotAssignmentController {
public:
  /// Создает controller таблицы employee_plot_assignment.
  ///@param db база данных приложения.
  explicit EmployeePlotAssignmentController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  EmployeePlotAssignmentRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  EmployeePlotAssignmentRowResult load(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  EmployeePlotAssignmentMutationResult create(const fasc::server::controllers::dto::EmployeePlotAssignmentCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  EmployeePlotAssignmentMutationResult update(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key,
                             const fasc::server::controllers::dto::EmployeePlotAssignmentUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  EmployeePlotAssignmentMutationResult erase(const fasc::server::controllers::dto::EmployeePlotAssignmentKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
