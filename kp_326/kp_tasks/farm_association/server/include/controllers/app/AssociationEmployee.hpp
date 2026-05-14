#pragma once

#include <controllers/app/FarmEntityError.hpp>
#include <controllers/dto/AssociationEmployee.hpp>
#include <database/Database.hpp>
#include <extended/fpp/Result.hpp>

namespace fasc::server::controllers::app {

using AssociationEmployeeRowsResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationEmployeeRowsDto, FarmEntityError>;
using AssociationEmployeeRowResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationEmployeeRowDto, FarmEntityError>;
using AssociationEmployeeMutationResult = fasc::extended::fpp::Result<fasc::server::controllers::dto::AssociationEmployeeMutationDto, FarmEntityError>;

/// Application controller таблицы association_employee.
class AssociationEmployeeController {
public:
  /// Создает controller таблицы association_employee.
  ///@param db база данных приложения.
  explicit AssociationEmployeeController(fasc::server::database::Database& db);

  /// Загружает список строк.
  ///@returns result со списком строк или ошибкой.
  AssociationEmployeeRowsResult list() const;

  /// Загружает строку.
  ///@param key первичный ключ строки.
  ///@returns result со строкой или ошибкой.
  AssociationEmployeeRowResult load(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const;

  /// Создает строку.
  ///@param dto данные создания.
  ///@returns result изменения или ошибка.
  AssociationEmployeeMutationResult create(const fasc::server::controllers::dto::AssociationEmployeeCreateDto& dto) const;

  /// Обновляет строку.
  ///@param key первичный ключ строки.
  ///@param dto данные обновления.
  ///@returns result изменения или ошибка.
  AssociationEmployeeMutationResult update(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key,
                             const fasc::server::controllers::dto::AssociationEmployeeUpdateDto& dto) const;

  /// Удаляет строку.
  ///@param key первичный ключ строки.
  ///@returns result изменения или ошибка.
  AssociationEmployeeMutationResult erase(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const;

private:
  fasc::server::database::Database& db_;
};

} // namespace fasc::server::controllers::app
