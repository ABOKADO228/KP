#pragma once

#include <controllers/app/FarmEmployee.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmEmployee.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmEmployeeRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmEmployeeRowsView, fasc::server::views::ErrorView>;
using FarmEmployeeRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmEmployeeRowView, fasc::server::views::ErrorView>;
using FarmEmployeeMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmEmployeeMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_employee.
class FarmEmployeeHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmEmployeeHttpController(fasc::server::controllers::app::FarmEmployeeController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmEmployeeRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmEmployeeRowViewResult load(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmEmployeeMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmEmployeeMutationViewResult update(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmEmployeeMutationViewResult erase(const fasc::server::controllers::dto::FarmEmployeeKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmEmployeeController& controller_;
};

} // namespace fasc::server::controllers::http
