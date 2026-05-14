#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/EmploymentStatus.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using EmploymentStatusRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::EmploymentStatusRowsDto, FarmEntityError>;
using EmploymentStatusRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::EmploymentStatusRowDto, FarmEntityError>;
using EmploymentStatusMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::EmploymentStatusMutationDto, FarmEntityError>;

/// Application controller таблицы employment_status.
class EmploymentStatusController {
public:
  /// Создает controller таблицы employment_status.
  ///@param db база данных приложения.
  explicit EmploymentStatusController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  EmploymentStatusRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  EmploymentStatusRowResult load(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  EmploymentStatusMutationResult create(const fasc::server::controllers::dto::EmploymentStatusCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  EmploymentStatusMutationResult update(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key,
                             const fasc::server::controllers::dto::EmploymentStatusUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  EmploymentStatusMutationResult erase(const fasc::server::controllers::dto::EmploymentStatusKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
