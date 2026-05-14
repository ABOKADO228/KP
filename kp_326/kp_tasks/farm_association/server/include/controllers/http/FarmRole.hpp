#pragma once

#include <controllers/app/FarmRole.hpp>
#include <extended/fpp/Result.hpp>
#include <views/Error.hpp>
#include <views/FarmRole.hpp>

#include <string_view>

namespace fasc::server::controllers::http {

using FarmRoleRowsViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmRoleRowsView, fasc::server::views::ErrorView>;
using FarmRoleRowViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmRoleRowView, fasc::server::views::ErrorView>;
using FarmRoleMutationViewResult = fasc::extended::fpp::Result<fasc::server::views::FarmRoleMutationView, fasc::server::views::ErrorView>;

/// HTTP controller таблицы farm_role.
class FarmRoleHttpController {
public:
  /// Создает HTTP controller.
  ///@param controller application controller таблицы.
  explicit FarmRoleHttpController(fasc::server::controllers::app::FarmRoleController& controller);

  /// Готовит view списка.
  ///@returns view-result списка.
  FarmRoleRowsViewResult list() const;

  /// Готовит view строки.
  ///@param key первичный ключ строки.
  ///@returns view-result строки.
  FarmRoleRowViewResult load(const fasc::server::controllers::dto::FarmRoleKeyDto& key) const;

  /// Готовит view создания.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmRoleMutationViewResult create(std::string_view body) const;

  /// Готовит view обновления.
  ///@param key первичный ключ строки.
  ///@param body JSON-тело.
  ///@returns view-result изменения.
  FarmRoleMutationViewResult update(const fasc::server::controllers::dto::FarmRoleKeyDto& key,
                                 std::string_view body) const;

  /// Готовит view удаления.
  ///@param key первичный ключ строки.
  ///@returns view-result изменения.
  FarmRoleMutationViewResult erase(const fasc::server::controllers::dto::FarmRoleKeyDto& key) const;

private:
  fasc::server::controllers::app::FarmRoleController& controller_;
};

} // namespace fasc::server::controllers::http
