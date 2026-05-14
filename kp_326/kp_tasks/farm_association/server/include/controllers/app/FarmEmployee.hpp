#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/FarmEmployee.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using FarmEmployeeRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmEmployeeRowsDto, FarmEntityError>;
using FarmEmployeeRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmEmployeeRowDto, FarmEntityError>;
using FarmEmployeeMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::FarmEmployeeMutationDto, FarmEntityError>;

/// Application controller таблицы farm_employee.
class FarmEmployeeController {
public:
  /// Создает controller таблицы farm_employee.
  ///@param db база данных приложения.
  explicit FarmEmployeeController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  FarmEmployeeRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  FarmEmployeeRowResult load(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  FarmEmployeeMutationResult create(const fasc::server::controllers::dto::FarmEmployeeCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  FarmEmployeeMutationResult update(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key,
                             const fasc::server::controllers::dto::FarmEmployeeUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  FarmEmployeeMutationResult erase(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
