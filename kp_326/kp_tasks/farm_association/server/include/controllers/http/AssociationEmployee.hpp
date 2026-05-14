#pragma once

#include <controllers/app/AssociationEmployee.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/AssociationEmployee.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using AssociationEmployeeRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationEmployeeRowsView, fasc::server::views::ErrorView>;
using AssociationEmployeeRowViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationEmployeeRowView, fasc::server::views::ErrorView>;
using AssociationEmployeeMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationEmployeeMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы association_employee.
class AssociationEmployeeHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit AssociationEmployeeHttpController(fasc::server::controllers::app::AssociationEmployeeController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  AssociationEmployeeRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  AssociationEmployeeRowViewResult load(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationEmployeeMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationEmployeeMutationViewResult update(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  AssociationEmployeeMutationViewResult erase(const fasc::server::controllers::dto::AssociationEmployeeKeyDto& key) const;

private:
  fasc::server::controllers::app::AssociationEmployeeController& controller_;
};

} // namespace fasc::server::controllers::http
