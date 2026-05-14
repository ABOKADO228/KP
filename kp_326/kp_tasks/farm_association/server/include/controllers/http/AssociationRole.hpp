#pragma once

#include <controllers/app/AssociationRole.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/AssociationRole.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using AssociationRoleRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationRoleRowsView, fasc::server::views::ErrorView>;
using AssociationRoleRowViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationRoleRowView, fasc::server::views::ErrorView>;
using AssociationRoleMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::AssociationRoleMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы association_role.
class AssociationRoleHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit AssociationRoleHttpController(fasc::server::controllers::app::AssociationRoleController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  AssociationRoleRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  AssociationRoleRowViewResult load(const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationRoleMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  AssociationRoleMutationViewResult update(const fasc::server::controllers::dto::AssociationRoleKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  AssociationRoleMutationViewResult erase(const fasc::server::controllers::dto::AssociationRoleKeyDto& key) const;

private:
  fasc::server::controllers::app::AssociationRoleController& controller_;
};

} // namespace fasc::server::controllers::http
